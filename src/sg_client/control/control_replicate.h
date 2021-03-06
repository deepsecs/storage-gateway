/**********************************************
* Copyright (c) 2016 Huawei Technologies Co., Ltd. All rights reserved.
* 
* File name:    replicate_control.h
* Author: 
* Date:         2017/01/11
* Version:      1.0
* Description:
* 
************************************************/
#ifndef CONTROL_REPLICATE_H_
#define CONTROL_REPLICATE_H_
#include <string>
#include <map>
#include <memory>
#include <boost/uuid/uuid_generators.hpp>
#include "rpc/clients/replicate_inner_ctrl_client.h"
#include "rpc/replicate_control.grpc.pb.h"
#include "sg_client/volume.h"
#include "rpc/clients/volume_inner_ctrl_client.h"

using grpc::ServerContext;
using grpc::Status;
using huawei::proto::control::CreateReplicationReq;
using huawei::proto::control::ReplicationCommonRes;
using huawei::proto::control::EnableReplicationReq;
using huawei::proto::control::DisableReplicationReq;
using huawei::proto::control::FailoverReplicationReq;
using huawei::proto::control::ReverseReplicationReq;
using huawei::proto::control::DeleteReplicationReq;
using huawei::proto::ReplicateOperation;

class ReplicateCtrl:public huawei::proto::control::ReplicateControl::Service {
    Status CreateReplication(ServerContext* context,
                             const CreateReplicationReq* request,
                             ReplicationCommonRes* response);
    Status EnableReplication(ServerContext* context,
                             const EnableReplicationReq* request,
                             ReplicationCommonRes* response);
    Status DisableReplication(ServerContext* context,
                              const DisableReplicationReq* request,
                              ReplicationCommonRes* response);
    Status FailoverReplication(ServerContext* context,
                               const FailoverReplicationReq* request,
                                      ReplicationCommonRes* response);
    Status ReverseReplication(ServerContext* context,
                              const ReverseReplicationReq* request,
                              ReplicationCommonRes* response);
    Status DeleteReplication(ServerContext* context,
                             const DeleteReplicationReq* request,
                             ReplicationCommonRes* response);
    std::shared_ptr<ReplicateProxy> get_replicate_proxy(const string& vol_name);
    std::shared_ptr<JournalWriter> get_journal_writer(const string& vol_name);
    void update_volume_attr(const string& volume);

    StatusCode do_replicate_operation(const string& vol,
        const string& op_id, const RepRole& role,const ReplicateOperation& op,
        const string& snap_name);

 public:
    explicit ReplicateCtrl(std::map<string, std::shared_ptr<Volume>>& volumes);
    ~ReplicateCtrl();
 private:
    std::unique_ptr<RepInnerCtrlClient> rep_ctrl_client_;
    std::unique_ptr<VolInnerCtrlClient> vol_ctrl_client_;
    std::map<string, std::shared_ptr<Volume>>& volumes_;
    boost::uuids::random_generator uuid_generator_;
};

#endif
