// MapHistory.cpp

#include "open_viii/graphics/background/MapHistory.hpp"
#include <boost/ut.hpp>
#include <fmt/format.h>

int
  main()
{
  using namespace boost::ut;
  using namespace boost::ut::literals;
  using namespace boost::ut::operators::terse;

  using namespace open_viii::graphics::background;

  [[maybe_unused]] suite tests = [] {
    "default constructed MapHistory is empty"_test = [] {
      const MapHistory history{};

      expect(eq(history.count(), std::size_t{ 0 }));
      expect(eq(history.redo_count(), std::size_t{ 0 }));

      expect(!history.undo_enabled());
      expect(!history.redo_enabled());
    };

    "current undo state is empty when no history exists"_test = [] {
      const MapHistory history{};

      expect(eq(history.current_undo_pushed(),
                MapHistory::pushed::unknown));

      expect(history.current_undo_description().empty());
    };

    "current redo state is empty when no history exists"_test = [] {
      const MapHistory history{};

      expect(eq(history.current_redo_pushed(),
                MapHistory::pushed::unknown));

      expect(history.current_redo_description().empty());
    };

    "copy_working creates undo entry"_test = [] {
      MapHistory history{};

      (void)history.copy_working("working change");

      expect(eq(history.count(), std::size_t{ 1 }));
      expect(history.undo_enabled());

      expect(eq(history.current_undo_pushed(),
                MapHistory::pushed::working));

      expect(eq(history.current_undo_description(),
                std::string_view{ "working change" }));
    };

    "copy_original creates undo entry"_test = [] {
      MapHistory history{};

      (void)history.copy_original("original change");

      expect(eq(history.count(), std::size_t{ 1 }));

      expect(eq(history.current_undo_pushed(),
                MapHistory::pushed::original));

      expect(eq(history.current_undo_description(),
                std::string_view{ "original change" }));
    };

    "undo transfers state to redo history"_test = [] {
      MapHistory history{};

      (void)history.copy_working("change");

      expect(history.undo());

      expect(eq(history.count(), std::size_t{ 0 }));
      expect(eq(history.redo_count(), std::size_t{ 1 }));

      expect(history.redo_enabled());

      expect(eq(history.current_redo_description(),
                std::string_view{ "change" }));

      expect(eq(history.current_redo_pushed(),
                MapHistory::pushed::working));
    };

    "redo restores undo history"_test = [] {
      MapHistory history{};

      (void)history.copy_working("change");

      expect(history.undo());
      expect(history.redo());

      expect(eq(history.count(), std::size_t{ 1 }));
      expect(eq(history.redo_count(), std::size_t{ 0 }));

      expect(history.undo_enabled());
      expect(!history.redo_enabled());
    };

    "clear_redo removes redo history"_test = [] {
      MapHistory history{};

      (void)history.copy_working("change");

      expect(history.undo());

      history.clear_redo();

      expect(eq(history.redo_count(), std::size_t{ 0 }));
      expect(!history.redo_enabled());
    };

    "begin_multi_frame_working creates single undo entry"_test = [] {
      MapHistory history{};

      (void)history.begin_multi_frame_working("multi frame");

      (void)history.copy_working("ignored");
      (void)history.copy_working("ignored");

      history.end_multi_frame_working();

      expect(eq(history.count(), std::size_t{ 1 }));

      expect(eq(history.current_undo_description(),
                std::string_view{ "multi frame" }));
    };

    "end_multi_frame_working updates description"_test = [] {
      MapHistory history{};

      (void)history.begin_multi_frame_working("before");

      history.end_multi_frame_working("after");

      expect(eq(history.current_undo_description(),
                std::string_view{ "after" }));
    };

    "undo returns false when history is empty"_test = [] {
      MapHistory history{};

      expect(!history.undo());
    };

    "redo returns false when redo history is empty"_test = [] {
      MapHistory history{};

      expect(!history.redo());
    };

    "formatter outputs pushed enum names"_test = [] {
      using namespace std::string_literals;

      expect(eq(fmt::format("{}", MapHistory::pushed::unknown),
                "Unknown"s));

      expect(eq(fmt::format("{}", MapHistory::pushed::original),
                "Original"s));

      expect(eq(fmt::format("{}", MapHistory::pushed::working),
                "Working"s));
    };
  };
}