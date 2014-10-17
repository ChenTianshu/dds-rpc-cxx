#ifndef OMG_DDS_RPC_COMMON_H
#define OMG_DDS_RPC_COMMON_H

struct DDS_SampleIdentity_t;

bool operator < (
  const DDS_SampleIdentity_t & lhs,
  const DDS_SampleIdentity_t & rhs);

namespace dds {

  class SampleIdentity_t;

  bool operator < (
    const dds::SampleIdentity_t & lhs,
    const dds::SampleIdentity_t & rhs);

}

#endif // OMG_DDS_RPC_COMMON_H