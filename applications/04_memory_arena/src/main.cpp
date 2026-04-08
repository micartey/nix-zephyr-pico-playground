#include <cstdint>
#include <vector>
#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/shell/shell.h>
#include "zephyr/sys/printk.h"
#include <memory_resource>
#include <memory>
#include <iostream>

#include "bootloader_cmd.h"

static int cmd_hello(const struct shell *sh, int argc, char **argv) {
  printk("World!\n");
  return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_demo,
        SHELL_CMD(ping, NULL, "Ping command.", +[](const struct shell *sh, int argc, char **argv) -> int {
            ARG_UNUSED(sh);
            ARG_UNUSED(argc);
            ARG_UNUSED(argv);

            printk("Pong!\n");
            return 0;
        }),
        SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(hello, &sub_demo, "Print \"World!\"", cmd_hello);

/**
 * A custom pool arena that keeps track of usage and supports individual deallocation
 */
class tracked_pool_arena : public std::pmr::unsynchronized_pool_resource {
    size_t m_allocated;

public:
    tracked_pool_arena()
        : std::pmr::unsynchronized_pool_resource(std::pmr::get_default_resource()),
          m_allocated(0) {}

    void* do_allocate(size_t bytes, size_t alignment) override {
        void* p = std::pmr::unsynchronized_pool_resource::do_allocate(bytes, alignment);
        if (p) {
            m_allocated += bytes;
        }
        return p;
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override {
        std::pmr::unsynchronized_pool_resource::do_deallocate(p, bytes, alignment);
        m_allocated -= bytes;
    }

    void release_all() {
        std::pmr::unsynchronized_pool_resource::release();
        m_allocated = 0;
    }

    size_t allocated() const { return m_allocated; }
};

struct Poiint {
    uint32_t test;

public:
    Poiint(uint32_t value) {
        test = value;
    }

    uint32_t get() {
        return test;
    }
};

int main(void) {
    k_sleep(K_SECONDS(5));

    tracked_pool_arena arena;

    try {
        std::cout << "Arena allocated: " << arena.allocated() << " bytes\n";

        // Allocate string in its own scope to free memory before Poiint allocation
        {
            // Use a long string to bypass Small String Optimization (SSO)
            std::pmr::string str("This is a longer string that will definitely allocate", &arena);
            std::cout << "Allocated string. Allocated: " << arena.allocated() << " bytes\n";
        } // str is destroyed here - memory is freed back to pool

        std::cout << "After string scope. Allocated: " << arena.allocated() << " bytes\n";

        auto* adasd = std::pmr::polymorphic_allocator<Poiint>(&arena).allocate(1);
        new (adasd) Poiint(10);

        std::cout << "Allocated Poiint in arena. Allocated: " << arena.allocated() << " bytes\n";

        std::pmr::vector<Poiint> poiint(&arena);
        poiint.push_back(*adasd);
        std::cout << "After push_back. Allocated: " << arena.allocated() << " bytes\n";

    } catch (const std::bad_alloc& e) {
        std::cerr << "Allocation failed: " << e.what() << '\n';
    }

    // This reclaims ALL memory (resets arena)
    arena.release_all();

  while (true) {
    k_sleep(K_FOREVER);
  }

  (void)cmd_bootloader;
  return 0;
}
