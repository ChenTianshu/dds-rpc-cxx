#include "RobotControlSupport.h"

namespace robot {

  RobotControlSupport::Client::Client()
    : dds::rpc::ClientEndpoint(
        new dds::rpc::details::ClientImpl<robot::RobotControl>(), 0)
  {
      if (!impl_)
        throw std::runtime_error("Could not create robot::RobotControl::Client");
  }

  RobotControlSupport::Client::Client(
    const dds::rpc::ClientParams & client_params)
    : dds::rpc::ClientEndpoint(
        new dds::rpc::details::ClientImpl<robot::RobotControl>(client_params), 0)
  {
    if (!impl_)
      throw std::runtime_error("Could not create robot::RobotControl::Client");
  }

  /***********************************************/
  /* Sync methods */
  /***********************************************/
  void RobotControlSupport::Client::command(const Command & command)
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->command(command);
  }

  float RobotControlSupport::Client::setSpeed(float speed)
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->setSpeed(speed);
  }

  float RobotControlSupport::Client::getSpeed()
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->getSpeed();
  }

  void RobotControlSupport::Client::getStatus(Status & status)
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->getStatus(status);
  }


  /***********************************************/
  /* Async methods */
  /***********************************************/
  dds::rpc::future<void> 
    RobotControlSupport::Client::command_async(const robot::Command & command)
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->command_async(command);
  }

  dds::rpc::future<float> 
    RobotControlSupport::Client::setSpeed_async(float speed)
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->setSpeed_async(speed);
  }

  dds::rpc::future<float> 
    RobotControlSupport::Client::getSpeed_async()
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->getSpeed_async();
  }

  dds::rpc::future<robot::RobotControl_getStatus_Out> 
    RobotControlSupport::Client::getStatus_async()
  {
    auto impl = static_cast<dds::rpc::details::ClientImpl<robot::RobotControl> *>(impl_.get());
    return impl->getStatus_async();
  }


} // namespace robot


namespace dds {
  namespace rpc {
    namespace details {

      static dds::SampleIdentity to_rpc_sample_identity(const DDS::SampleIdentity_t & inid)
      {
        static_assert(sizeof(dds::SampleIdentity) == sizeof(DDS::SampleIdentity_t),
                      "Sizes of two SampleIdentity don't match!");

        dds::SampleIdentity outid;
        memcpy(&outid, &inid, sizeof(SampleIdentity_t));
        return outid;
      }

      rpc::ReplierParams
        to_replier_params(const rpc::ServiceParams & service_params)
      {
        return rpc::ReplierParams()
                 .domain_participant(service_params.domain_participant())
                 .service_name(service_params.service_name());
      }

      Dispatcher<robot::RobotControl>::Dispatcher(robot::RobotControl & service_impl)
        : robotimpl_(&service_impl),
          replier_(to_replier_params(ServiceParams().service_name("RobotControl")))
      { }

      Dispatcher<robot::RobotControl>::Dispatcher(
            robot::RobotControl & service_impl,
            const ServiceParams & service_params)
        : robotimpl_(&service_impl),
          replier_(to_replier_params(service_params))
      { }

      void Dispatcher<robot::RobotControl>::close()
      {}

      bool is_command(const Sample<robot::RobotControl_Request> & request_sample)
      {
        return request_sample.data().data._d == robot::RobotControl_command_Hash;
      }

      bool is_getSpeed(const Sample<robot::RobotControl_Request> & request_sample)
      {
        return request_sample.data().data._d == robot::RobotControl_getSpeed_Hash;
      }

      bool is_setSpeed(const Sample<robot::RobotControl_Request> & request_sample)
      {
        return request_sample.data().data._d == robot::RobotControl_setSpeed_Hash;
      }

      bool is_getStatus(const Sample<robot::RobotControl_Request> & request_sample)
      {
        return request_sample.data().data._d == robot::RobotControl_getStatus_Hash;
      }

      helper::unique_data<robot::RobotControl_Reply> do_command(
        const Sample<robot::RobotControl_Request> & request_sample,
        robot::RobotControl * service_impl)
      {
        helper::unique_data<robot::RobotControl_Reply> reply;

        service_impl->command(request_sample.data().data._u.command.com);

        reply->data._d = robot::RobotControl_command_Hash;
        reply->data._u.command._d = dds::rpc::REMOTE_EX_OK;
        reply->data._u.command._u.result.dummy = 0;

        return reply;
      }

      helper::unique_data<robot::RobotControl_Reply> do_setSpeed(
        const Sample<robot::RobotControl_Request> & request_sample,
        robot::RobotControl * service_impl)
      {
        helper::unique_data<robot::RobotControl_Reply> reply;

        try
        {
          float speed =
            service_impl->setSpeed(request_sample.data().data._u.setSpeed.speed);

          reply->data._d = robot::RobotControl_setSpeed_Hash;
          reply->data._u.setSpeed._d = dds::rpc::REMOTE_EX_OK;
          reply->data._u.setSpeed._u.result.return_ = speed;
        }
        catch (robot::TooFast & toofast)
        {
          reply->data._d = robot::RobotControl_setSpeed_Hash;
          reply->data._u.setSpeed._d = robot::TooFast_Ex_Hash;
          reply->data._u.setSpeed._u.toofast_ex = toofast;
        }

        return reply;
      }

      helper::unique_data<robot::RobotControl_Reply> do_getSpeed(
        const Sample<robot::RobotControl_Request> & request_sample,
        robot::RobotControl * service_impl)
      {
        helper::unique_data<robot::RobotControl_Reply> reply;

        float speed = service_impl->getSpeed();

        reply->data._d = robot::RobotControl_getSpeed_Hash;
        reply->data._u.getSpeed._d = dds::rpc::REMOTE_EX_OK;
        reply->data._u.getSpeed._u.result.return_ = speed;

        return reply;
      }

      helper::unique_data<robot::RobotControl_Reply> do_getStatus(
        const Sample<robot::RobotControl_Request> & request_sample,
        robot::RobotControl * service_impl)
      {
        helper::unique_data<robot::RobotControl_Reply> reply;

        reply->data._d = robot::RobotControl_getStatus_Hash;
        reply->data._u.getStatus._d = dds::rpc::REMOTE_EX_OK;
        service_impl->getStatus(reply->data._u.getStatus._u.result.status);

        return reply;
      }

      void Dispatcher<robot::RobotControl>::dispatch(const dds::Duration & timeout)
      {
        Sample<RequestType> request_sample;
        helper::unique_data<ReplyType> reply;

        if (replier_.receive_request(request_sample, timeout))
        {
          if (is_command(request_sample))
            reply = do_command(request_sample, robotimpl_);
          else if (is_getSpeed(request_sample))
            reply = do_getSpeed(request_sample, robotimpl_);
          else if (is_setSpeed(request_sample))
            reply = do_setSpeed(request_sample, robotimpl_);
          else if (is_getStatus(request_sample))
            reply = do_getStatus(request_sample, robotimpl_);
          else
          {
            reply->header.remoteEx = dds::rpc::REMOTE_EX_UNKNOWN_OPERATION;
            reply->data._d = 0; // default
          }

          replier_.send_reply(*reply, to_rpc_sample_identity(request_sample.identity()));
        }
        else
          printf("timeout or invalid sampleinfo. Ignoring...\n");
      }

      void Dispatcher<robot::RobotControl>::run_impl(const dds::Duration & timeout)
      {
        dispatch(timeout);
      }

      /***************************************************************************/
      /* ClientImpl */
      /***************************************************************************/

      static dds::rpc::RequesterParams 
        to_requester_params(const ClientParams & client_params)
      {        
        return dds::rpc::RequesterParams()
          .domain_participant(client_params.domain_participant())
          .service_name(client_params.service_name());
      }

      ClientImpl<robot::RobotControl>::ClientImpl() 
        : params_(dds::rpc::ClientParams().service_name("RobotControl")),
          requester_(to_requester_params(params_))
      {  }

      ClientImpl<robot::RobotControl>::ClientImpl(
        const dds::rpc::ClientParams & client_params)
        : params_(client_params),
          requester_(to_requester_params(params_))
      { }

      void ClientImpl<robot::RobotControl>::bind(const std::string & instance_name)
      {
        requester_.bind(instance_name);
      }

      void ClientImpl<robot::RobotControl>::unbind()
      {
        requester_.unbind();
      }

      bool ClientImpl<robot::RobotControl>::is_bound() const
      {
        return requester_.is_bound();
      }

      std::string ClientImpl<robot::RobotControl>::get_bound_instance_name() const
      {
        return requester_.get_bound_instance_name();
      }

      std::vector<std::string> 
        ClientImpl<robot::RobotControl>::get_discovered_service_instances() const
      {
          return std::vector<std::string>();
      }

      void ClientImpl<robot::RobotControl>::wait_for_service()
      { }

      void ClientImpl<robot::RobotControl>::wait_for_service(
        const dds::Duration & maxWait) 
      { }

      void ClientImpl<robot::RobotControl>::wait_for_service(
        std::string instanceName)
      { }

      void ClientImpl<robot::RobotControl>::wait_for_service(
        const dds::Duration & maxWait,
        std::string instanceName)
      { }

      void ClientImpl<robot::RobotControl>::wait_for_services(int count)
      { }
      
      void ClientImpl<robot::RobotControl>::wait_for_services(
        const dds::Duration & maxWait,
        int count)
      { }

      void ClientImpl<robot::RobotControl>::wait_for_services(
        const std::vector<std::string> & instanceNames)
      { }
      
      void ClientImpl<robot::RobotControl>::wait_for_services(
        const dds::Duration & maxWait,
        const std::vector<std::string> & instanceNames)
      { }

      future<void> 
        ClientImpl<robot::RobotControl>::wait_for_service_async()
      {    
        return future<void>();
      }

      future<void> 
        ClientImpl<robot::RobotControl>::wait_for_service_async(
        std::string instanceName)
      {
        return future<void>();
      }

      future<void> 
        ClientImpl<robot::RobotControl>::wait_for_services_async(
        int count)
      {
          return future<void>();
      }
      
      future<void> 
        ClientImpl<robot::RobotControl>::wait_for_services_async(
        const std::vector<std::string> & instanceNames)
      {
          return future<void>();
      }

      DDS::DataWriter * ClientImpl<robot::RobotControl>::get_request_datawriter() const
      {
        return requester_.get_request_datawriter();
      }

      DDS::DataReader * ClientImpl<robot::RobotControl>::get_reply_datareader() const
      {
        return requester_.get_reply_datareader();
      }

      ClientParams ClientImpl<robot::RobotControl>::get_client_params() const
      {
        return params_;
      }

      void ClientImpl<robot::RobotControl>::close() 
      { }

      void ClientImpl<robot::RobotControl>::command(const robot::Command & command)
      {
        helper::unique_data<robot::RobotControl_Request> request;
        Sample<robot::RobotControl_Reply> reply_sample;

        request->data._d = robot::RobotControl_command_Hash;
        request->data._u.command.com = command;

        requester_.send_request(*request);
        requester_.receive_reply(reply_sample, 
                                 request->header.requestId,
                                 dds::Duration::from_seconds(20));
        printf("reply received successfully from command %d\n",
          reply_sample.data().header.relatedRequestId.sequence_number.low);
      }

      float ClientImpl<robot::RobotControl>::setSpeed(float speed)
      {
        helper::unique_data<robot::RobotControl_Request> request;
        Sample<robot::RobotControl_Reply> reply_sample;

        request->data._d = robot::RobotControl_setSpeed_Hash;
        request->data._u.setSpeed.speed = speed;

        requester_.send_request(*request);
        requester_.receive_reply(reply_sample, 
                                 request->header.requestId,
                                 dds::Duration::from_seconds(20));

        if (reply_sample.data().data._d == robot::RobotControl_setSpeed_Hash)
        {
          printf("reply received successfully from setSpeed %d\n",
            reply_sample.data().header.relatedRequestId.sequence_number.low);

          switch (reply_sample.data().data._u.setSpeed._d)
          {
          case dds::rpc::REMOTE_EX_OK:
            {
                return reply_sample.data().data._u.setSpeed._u.result.return_;
                break;
            }
            case robot::TooFast_Ex_Hash:
            {
                throw reply_sample.data().data._u.setSpeed._u.toofast_ex;
                break;
            }
            default:
            {
                throw std::runtime_error("Received unknown exception from setSpeed.");
                break;
            }
          }
        }
        else
        {
          throw std::runtime_error("Received unknown response for setSpeed.\n");
        }
      }

      float ClientImpl<robot::RobotControl>::getSpeed()
      {
        helper::unique_data<robot::RobotControl_Request> request;
        Sample<robot::RobotControl_Reply> reply_sample;

        request->data._d = robot::RobotControl_getSpeed_Hash;
        request->data._u.getSpeed.dummy = 0;

        requester_.send_request(*request);
        requester_.receive_reply(reply_sample, 
                                 request->header.requestId,
                                 dds::Duration::from_seconds(20));

        if (reply_sample.data().data._d == robot::RobotControl_getSpeed_Hash)
        {
          printf("reply received successfully from getSpeed %d\n",
            reply_sample.data().header.relatedRequestId.sequence_number.low);

          switch (reply_sample.data().data._u.getSpeed._d)
          {
          case dds::rpc::REMOTE_EX_OK:
            {
              return reply_sample.data().data._u.getSpeed._u.result.return_;
              break;
            }
            default:
            {
              throw std::runtime_error("Received unknown exception from getSpeed.");
              break;
            }
          }
        }
        else
        {
          throw std::runtime_error("Received unknown response for getSpeed.\n");
        }
      }

      void ClientImpl<robot::RobotControl>::getStatus(robot::Status & status)
      {
        helper::unique_data<robot::RobotControl_Request> request;
        Sample<robot::RobotControl_Reply> reply_sample;

        request->data._d = robot::RobotControl_getStatus_Hash;
        request->data._u.getStatus.dummy = 0;

        requester_.send_request(*request);
        requester_.receive_reply(reply_sample, 
                                 request->header.requestId,
                                 dds::Duration::from_seconds(20));

        if (reply_sample.data().data._d == robot::RobotControl_getStatus_Hash)
        {
          printf("reply received successfully from getStatus %d\n",
            reply_sample.data().header.relatedRequestId.sequence_number.low);

          switch (reply_sample.data().data._u.getSpeed._d)
          {
            case dds::rpc::REMOTE_EX_OK:
            {
              robot::Status_copy(&status, &reply_sample.data().data._u.getStatus._u.result.status);
              break;
            }
            default:
            {
              throw std::runtime_error("Received unknown exception from getStatus.");
              break;
            }
          }
        }
        else
        {
          throw std::runtime_error("Received unknown response for getStatus.\n");
        }
      }

      dds::rpc::future<void> 
        ClientImpl<robot::RobotControl>::command_async(
          const robot::Command & command)
      {
        helper::unique_data<robot::RobotControl_Request> request;
        Sample<robot::RobotControl_Reply> reply_sample;

        request->data._d = robot::RobotControl_command_Hash;
        request->data._u.getSpeed.dummy = 0;

        return
          requester_
            .send_request_async(*request)
            .then([](dds::rpc::future <Sample<robot::RobotControl_Reply>> && reply) {
              reply.get();
            });
      }

      dds::rpc::future<float> 
        ClientImpl<robot::RobotControl>::setSpeed_async(float speed)
      {
        helper::unique_data<robot::RobotControl_Request> request;
        Sample<robot::RobotControl_Reply> reply_sample;

        request->data._d = robot::RobotControl_setSpeed_Hash;
        request->data._u.setSpeed.speed = speed;

        return
          requester_
            .send_request_async(*request)
            .then([](dds::rpc::future <Sample<robot::RobotControl_Reply>> && reply_fut) {
                    Sample<robot::RobotControl_Reply> reply_sample = reply_fut.get();
                    if (reply_sample.data().data._u.setSpeed._d == robot::TooFast_Ex_Hash)
                    {
                      throw robot::TooFast();
                    }
                    else 
                      return reply_sample.data().data._u.setSpeed._u.result.return_;
            });
      }
      
      dds::rpc::future<float> 
        ClientImpl<robot::RobotControl>::getSpeed_async()
      {
          helper::unique_data<robot::RobotControl_Request> request;
          Sample<robot::RobotControl_Reply> reply_sample;

          request->data._d = robot::RobotControl_getSpeed_Hash;
          request->data._u.getSpeed.dummy = 0;

          return 
          requester_.send_request_async(*request)
                    .then([](dds::rpc::future <Sample<robot::RobotControl_Reply>> && reply) {
                        return reply.get().data().data._u.getSpeed._u.result.return_;
                    });
      }

      dds::rpc::future<robot::RobotControl_getStatus_Out> 
        ClientImpl<robot::RobotControl>::getStatus_async()
      {
        helper::unique_data<robot::RobotControl_Request> request;
        Sample<robot::RobotControl_Reply> reply_sample;

        request->data._d = robot::RobotControl_getStatus_Hash;
        request->data._u.getSpeed.dummy = 0;

        return
          requester_.send_request_async(*request)
            .then([](dds::rpc::future <Sample<robot::RobotControl_Reply>> && reply_fut) {
              return reply_fut.get().data().data._u.getStatus._u.result;
            });
      }

    } // namespace details
  } // namespace rpc
} // namespace dds

/*
Client::Client()
: impl_(boost::make_shared<details::ClientImpl>())
{}

*/