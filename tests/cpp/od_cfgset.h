#ifndef OD_CFGSET_H
#define OD_CFGSET_H

#include "od_cfgs.h"
#include <variant>

struct AnyCfg {
  std::variant<Cfg_1, Cfg_2, Cfg_3> cfg;

  template <typename CfgTy> CfgTy &get() { return std::get<CfgTy>(cfg); }

  auto index() const -> auto{ return cfg.index(); }

  template <typename CfgTy> AnyCfg(const CfgTy &c) : cfg(c) {}

  AnyCfg(){};
  AnyCfg(const AnyCfg &rhs) = default;
  /*
  AnyCfg &operator=(const AnyCfg &rhs) {
    cfg = rhs.cfg;
    return *this;
  }
  */
  AnyCfg &operator=(AnyCfg &&rhs) {
    cfg = std::move(rhs.cfg);
    return *this;
  }
};

template <size_t MAX_SIZE> struct ConfigurationsSet {
  size_t _size{0};
  bool _invalid{false};
  std::array<AnyCfg, MAX_SIZE> _confs;

  /*
  void add(const AnyCfg &c) {
    assert(_size < MAX_SIZE);
    _confs[_size++] = c;
  }
  */

  void add(AnyCfg &&c) {
    assert(_size < MAX_SIZE);
    _confs[_size++] = std::move(c);
  }

  void clear() { _size = 0; }

  void setInvalid() { _invalid = true; }
  bool invalid() const { return _invalid; }

  auto begin() -> auto{ return _confs.begin(); }
  auto end() -> auto{ return _confs.end(); }
};

#endif // OD_CFGSET_H
