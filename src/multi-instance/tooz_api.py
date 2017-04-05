from tooz import coordination
from tooz import hashring
import six, uuid
import time
import socket
import logging

logging.basicConfig(level=logging.DEBUG,
                format='%(asctime)s %(filename)s[line:%(lineno)d] %(levelname)s %(message)s',
                datefmt='%a, %d %b %Y %H:%M:%S',
                filename='tooz_api.log',
                filemode='w')
console = logging.StreamHandler()
console.setLevel(logging.INFO)
formatter = logging.Formatter('%(name)-12s: %(levelname)-8s %(message)s')
console.setFormatter(formatter)
logging.getLogger('').addHandler(console)

DEFAULT_BUCKET_NUMBER = 2**10
SOCKET_FILE = './sg_uds'
class Coordination():
    def __init__(self):
        self.sock = None
        self.hashring = None
        self.socket_file = SOCKET_FILE
        self.bucket_num = DEFAULT_BUCKET_NUMBER
        self.backend_url = None
        self._my_id = None

    def start(self, backend_url, member_id=None):
        self.backend_url = backend_url
        self._my_id = member_id or six.binary_type(six.text_type(uuid.uuid4()).encode('ascii'))
        logging.info("backend_url=%s", backend_url)
        logging.info("member_id=%s", self._my_id)
        try:
            self._coordinator = coordination.get_coordinator(backend_url, self._my_id)
            self._coordinator.start()
            logging.info("Coordination backend started successfully.")
        except coordination.ToozError:
            logging.exception("Error connecting to coordination backend.")

    def join_group(self, group_id):
        logging.debug('join group = %s', group_id)
        try:
            request = self._coordinator.join_group_create(group_id)
            if request:
                request.get()
        except coordination.ToozError:
            logging.exception("Error join group.")

    def leave_group(self, group_id):
        try:
            if self._coordinator:
                self._coordinator.leave_group(group_id)
        except coordination.ToozError:
            logging.exception("Error leave group.")

    def _join_group_callback(self, event):
        try:
            if self.sock is None:
                self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
                self.sock.connect(self.socket_file)
            self.sock.send("join_group")
            logging.debug('join group callback')
        except Exception:
            logging.exception('Error join group callback')

    def _leave_group_callback(self, event):
        if self.sock is None:
            self.sock = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
            self.sock.connect(self.socket_file)
        self.sock.send('leave_group')
        logging.debug('leave group callback')

    def get_members(self, group_id):
        if not self._coordinator:
            return []
        try:
            ret = self._coordinator.get_members(group_id)
            members = ret.get()
            logging.debug('members of group %s are: %s, Me: %s', group_id, members, self._my_id)
            return members
        except coordination.ToozError:
            logging.exception('Error getting members of group %s', group_id)
            return []

    def watch_group(self, group_id):
        logging.debug('watch_group group_id=%s', group_id)
        try:
            self._coordinator.watch_join_group(group_id, self._join_group_callback)
            self._coordinator.watch_leave_group(group_id, self._leave_group_callback)
        except coordination.ToozError:
            logging.exception("Error watch group: %s.", group_id)

    def stop(self):
        try:
            if self._coordinator:
                self._coordinator.stop()
        except coordination.ToozError:
            logging.exception("Error stop coordinator.")
        try:
            if self.sock:
                self.sock.close()
        except Exception:
            logging.exception("Error closing socket.")

    def run_watchers(self):
        try:
            if self._coordinator:
                self._coordinator.run_watchers()
                logging.debug('run_watcher')
        except coordination.ToozError:
            logging.exception("Error run watchers.")

    def heartbeat(self):
        if self._coordinator:
            if not self._coordinator.is_started:
                # re-connect
                self.start(self.backend_url, self._my_id)
            try:
                self._coordinator.heartbeat()
                logging.debug('heartbeat')
            except coordination.ToozError:
                logging.exception('Error sending a heartbeat to coordination backend.')

    #get nodes view remotely from tooz
    def refresh_nodes_view(self, group_id):
        try:
            members = self.get_members(group_id)
            self.hashring = hashring.HashRing(members)
        except coordination.ToozError:
                logging.exception('Error refresh nodes view.')

    #calculate node locally by hashring
    def get_nodes(self, data, ignore_nodes=None, replicas=1):
        if self.hashring is None:
            return []
        try:
            node_set = self.hashring.get_nodes(self.encode(data), ignore_nodes, replicas)
            return list(node_set)
        except coordination.ToozError:
            logging.exception('Error getting nodes.')

    def rehash_buckets_to_node(self, group_id, bucket_num=DEFAULT_BUCKET_NUMBER):
        """
        We have a fixed number of buckets (DEFAULT_BUCKET_NUMBER=1024),
        each node(also name as member) takes control part of buckets,
        We use hashring to calculate buckets belong to this node(member_id=self._myid)
        """
        node = self._my_id
        my_buckets=[]
        try:
            members = self.get_members(group_id)
            if self._my_id not in members:
                logging.warning("Member not in group, rejoin group")
                self.join_group(group_id)
                members = self.get_members(group_id)
                if self._my_id not in members:
                    logging.error("Can not get buckets because failed to join group")
                    return my_buckets
            hr = hashring.HashRing(members, partitions=100)
            for bucket in range(0, bucket_num):
                if node in hr.get_nodes(self.encode(str(bucket))):
                    my_buckets.append(str(bucket))

            logging.debug('my bucket: %s',[six.text_type(f) for f in my_buckets])
            logging.info('my bucket num: %d', len(my_buckets))
            return my_buckets
        except coordination.ToozError:
            logging.exception("Error getting group membership info from coordination backend")

    @staticmethod
    def encode(value):
        """encode to bytes"""
        return six.text_type(value).encode('utf-8')