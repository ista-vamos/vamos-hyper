#ifndef OD_CFGSET_H
#define OD_CFGSET_H

#include "od_cfgs.h"
#include <variant>

struct AnyCfg {
  std::variant<Cfg_1, Cfg_2, Cfg_3> cfg;

  template <typename CfgTy> CfgTy &get() { return std::get<CfgTy>(cfg); }

  auto index() const -> auto{ return cfg.index(); }

  template <typename CfgTy> AnyCfg(const CfgTy &c) : cfg(c) {}
};

template <size_t MAX_SIZE> struct ConfigurationsSet {
  std::vector<AnyCfg> _confs;
  bool _invalid{false};

  void add(const AnyCfg &c) { _confs.push_back(c); }

  void clear() { _confs.clear(); }

  void setInvalid() { _invalid = true; }
  bool invalid() const { return _invalid; }

  auto begin() -> auto{ return _confs.begin(); }
  auto end() -> auto{ return _confs.end(); }
};

#endif // OD_CFGSET_H
