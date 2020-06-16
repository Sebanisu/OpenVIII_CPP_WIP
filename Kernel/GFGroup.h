//
// Created by pcvii on 6/16/2020.
//

#ifndef VIIIARCHIVE_GFGROUP_H
#define VIIIARCHIVE_GFGROUP_H
template<typename T> struct GFGroup
{
private:
  T Quetzalcoatl_{};
  T Shiva_{};
  T Ifrit_{};
  T Siren_{};
  T Brothers_{};
  T Diablos_{};
  T Carbuncle_{};
  T Leviathan_{};
  T Pandemona_{};
  T Cerberus_{};
  T Alexander_{};
  T Doomtrain_{};
  T Bahamut_{};
  T Cactuar_{};
  T Tonberry_{};
  T Eden_{};

public:
  [[nodiscard]] T &Quetzalcoatl() const noexcept { return Quetzalcoatl_; }
  [[nodiscard]] T &Shiva() const noexcept { return Shiva_; }
  [[nodiscard]] T &Ifrit() const noexcept { return Ifrit_; }
  [[nodiscard]] T &Siren() const noexcept { return Siren_; }
  [[nodiscard]] T &Brothers() const noexcept { return Brothers_; }
  [[nodiscard]] T &Diablos() const noexcept { return Diablos_; }
  [[nodiscard]] T &Carbuncle() const noexcept { return Carbuncle_; }
  [[nodiscard]] T &Leviathan() const noexcept { return Leviathan_; }
  [[nodiscard]] T &Pandemona() const noexcept { return Pandemona_; }
  [[nodiscard]] T &Cerberus() const noexcept { return Cerberus_; }
  [[nodiscard]] T &Alexander() const noexcept { return Alexander_; }
  [[nodiscard]] T &Doomtrain() const noexcept { return Doomtrain_; }
  [[nodiscard]] T &Bahamut() const noexcept { return Bahamut_; }
  [[nodiscard]] T &Cactuar() const noexcept { return Cactuar_; }
  [[nodiscard]] T &Tonberry() const noexcept { return Tonberry_; }
  [[nodiscard]] T &Eden() const noexcept { return Eden_; }
};
#endif// VIIIARCHIVE_GFGROUP_H
