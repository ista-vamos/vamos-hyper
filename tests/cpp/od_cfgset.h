#ifndef OD_CFGSET_H
#define OD_CFGSET_H

#include <variant>
#include "od_cfgs.h"

struct ConfigurationsSet {
  using CfgTy = std::variant<Cfg_1, Cfg_2, Cfg_3>;

  std::vector<CfgTy> _confs;
  bool _invalid{false};

  void add(const CfgTy &c) { _confs.push_back(c); }

  void clear() { _confs.clear(); }

  void setInvalid() { _invalid = true; }
  bool invalid() const { return _invalid; }

  auto begin() -> auto{ return _confs.begin(); }
  auto end() -> auto{ return _confs.end(); }
};

#endif // OD_CFGSET_H
