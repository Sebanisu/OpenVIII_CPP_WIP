//
// Created by pcvii on 3/15/2023.
//
#include "open_viii/archive/Archives.hpp"
#include "open_viii/paths/Paths.hpp"
#include <boost/ut.hpp>// single header
int
  main()
{
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;
  using namespace boost::ut;
  using namespace open_viii::archive;
  [[maybe_unused]] suite tests = [] {
    open_viii::Paths::for_each_path([](const std::filesystem::path &path) {
      static constexpr auto coo = open_viii::LangT::en;
      const auto            archives
        = Archives(path, open_viii::LangCommon::to_string<coo>());
      if (!static_cast<bool>(archives)) {
        return;
      }
      "archive is loaded"_test = [&] {
        expect(static_cast<bool>(archives));
      };

      "archive correctly works in ranged for loop"_test = [&] {
        for ([[maybe_unused]] Archives::MyVariant item : archives) {
          expect(item.index() > 0);
          // std::visit([](std::reference_wrapper<std::optional<ZZZ>>){},item);
        }
      };

      "ArchivesIteratorVisitor correctly works in ranged for loop"_test = [&] {
        for (auto archive : archives) {
          ArchivesIteratorVisitor item = archive;
          std::invoke(item, [](FIFLFS<true> &) {
            // do something;
          });
          std::invoke(item, [](FIFLFS<false> &) {
            // do something;
          });
          std::invoke(item, [](std::optional<ZZZ> &) {
            // do something;
          });
          std::invoke(item, [](ZZZ &) {
            // do something;
          });
          std::invoke(item, [](const FIFLFS<true> &) {
            // do something;
          });
          std::invoke(item, [](const FIFLFS<false> &) {
            // do something;
          });
          std::invoke(item, [](const std::optional<ZZZ> &) {
            // do something;
          });
          std::invoke(item, [](const ZZZ &) {
            // do something;
          });
          // std::monostate,
          //     FIFLFSRef,
          //     FIFLFSConstRef,
          //     FIFLFSRef2,
          //     FIFLFSConstRef2,
          //     ZZZRef,
          //     ZZZConstRef > ;

          expect(
            std::invoke(
              item,
              [](FIFLFS<true> &) {
                return true;
              })
            or archive.index() != 1);
          expect(
            std::invoke(
              item,
              [](const FIFLFS<true> &fields) {
                for (const auto &field : fields) {
                  std::puts(field.get_base_name().data());
                }
                return true;
              })
            or archive.index() != 2);
          expect(
            std::invoke(
              item,
              [](FIFLFS<false> &) {
                return true;
              })
            or archive.index() != 3);
          expect(
            std::invoke(
              item,
              [](const FIFLFS<false> &) {
                return true;
              })
            or archive.index() != 4);
          expect(
            std::invoke(
              item,
              [](std::optional<ZZZ> &) {
                return true;
              })
            or archive.index() != 5);
          expect(
            std::invoke(
              item,
              [](ZZZ &) {
                return true;
              })
            or archive.index() != 5 or !std::get<5>(archive).get().has_value());
          expect(
            std::invoke(
              item,
              [](const std::optional<ZZZ> &) {
                return true;
              })
            or archive.index() != 6);
          expect(
            std::invoke(
              item,
              [](const ZZZ &) {
                return true;
              })
            or archive.index() != 6 or !std::get<6>(archive).get().has_value());
        }
      };
    });
  };
}