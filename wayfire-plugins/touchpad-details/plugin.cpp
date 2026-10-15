#include <string>

#include <wayfire/core.hpp>
#include <wayfire/input-device.hpp>
#include <wayfire/nonstd/wlroots-full.hpp>
#include <wayfire/plugin.hpp>
#include <wayfire/plugins/common/shared-core-data.hpp>
#include <wayfire/plugins/ipc/ipc-method-repository.hpp>

namespace touchpad_details
{
static std::string click_method_name(libinput_config_click_method method)
{
    switch (method)
    {
      case LIBINPUT_CONFIG_CLICK_METHOD_NONE:
        return "none";

      case LIBINPUT_CONFIG_CLICK_METHOD_BUTTON_AREAS:
        return "button-areas";

      case LIBINPUT_CONFIG_CLICK_METHOD_CLICKFINGER:
        return "clickfinger";

      default:
        return "default";
    }
}

static std::string scroll_method_name(libinput_config_scroll_method method)
{
    switch (method)
    {
      case LIBINPUT_CONFIG_SCROLL_NO_SCROLL:
        return "none";

      case LIBINPUT_CONFIG_SCROLL_2FG:
        return "two-finger";

      case LIBINPUT_CONFIG_SCROLL_EDGE:
        return "edge";

      case LIBINPUT_CONFIG_SCROLL_ON_BUTTON_DOWN:
        return "on-button-down";

      default:
        return "default";
    }
}

class plugin : public wf::plugin_interface_t
{
    wf::shared_data::ref_ptr_t<wf::ipc::method_repository_t> repository;

    wf::ipc::method_callback list = [] (const wf::json_t&)
    {
        wf::json_t touchpads = wf::json_t::array();
        for (auto& device : wf::get_core().get_input_devices())
        {
            auto *handle = device->get_wlr_handle();
            if (!wlr_input_device_is_libinput(handle))
            {
                continue;
            }

            auto *libinput = wlr_libinput_get_device_handle(handle);
            if (!libinput || (libinput_device_config_tap_get_finger_count(libinput) == 0))
            {
                continue;
            }

            wf::json_t touchpad;
            touchpad["name"] = libinput_device_get_name(libinput);
            touchpad["click-method"] =
                click_method_name(libinput_device_config_click_get_default_method(libinput));
            touchpad["scroll-method"] =
                scroll_method_name(libinput_device_config_scroll_get_default_method(libinput));
            touchpads.append(touchpad);
        }

        wf::json_t response;
        response["touchpads"] = touchpads;
        return response;
    };

  public:
    void init() override
    {
        repository->register_method("touchpad-details/list", list);
    }

    void fini() override
    {
        repository->unregister_method("touchpad-details/list");
    }
};
}

DECLARE_WAYFIRE_PLUGIN(touchpad_details::plugin);
