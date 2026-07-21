/*
 * Contact Sensor for Seeed Xiao nRF52840 with Reed Switch on D0
 */

#include "app_task.h"

#include "app/matter_init.h"
#include "app/task_executor.h"
#include "board/board.h"
#include "lib/core/CHIPError.h"
#include <app-common/zap-generated/attributes/Accessors.h>
#include <app/clusters/boolean-state-server/CodegenIntegration.h>

#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>

LOG_MODULE_DECLARE(app, CONFIG_CHIP_APP_LOG_LEVEL);

using namespace ::chip;
using namespace ::chip::app;
using namespace ::chip::DeviceLayer;

namespace {
    constexpr chip::EndpointId kContactSensorEndpointId = 1;

    /* D0 = P0.02 on Seeed Xiao nRF52840 */
    const struct gpio_dt_spec reed_gpio = {
    .port = DEVICE_DT_GET(DT_NODELABEL(gpio0)),
    .pin = 2,
    .dt_flags = GPIO_ACTIVE_LOW | GPIO_PULL_UP,
    };

} // namespace

static struct gpio_callback reed_cb_data;

static void ReedSwitchHandler(const struct device *dev, struct gpio_callback *cb, uint32_t pins)
{
    bool contact_detected = gpio_pin_get_dt(&reed_gpio) != 0;  // ACTIVE_LOW

    Nrf::PostTask([contact_detected] {
        auto *booleanState = Clusters::BooleanState::FindClusterOnEndpoint(kContactSensorEndpointId);
        if (booleanState != nullptr) {
            booleanState->SetStateValue(contact_detected);
            LOG_INF("Contact %s", contact_detected ? "DETECTED (closed)" : "OPEN");
        }
    });
}

CHIP_ERROR AppTask::Init()
{
    ReturnErrorOnFailure(Nrf::Matter::PrepareServer());

    if (!Nrf::GetBoard().Init(nullptr)) {
        LOG_ERR("Board init failed.");
        return CHIP_ERROR_INCORRECT_STATE;
    }

    ReturnErrorOnFailure(Nrf::Matter::RegisterEventHandler(Nrf::Board::DefaultMatterEventHandler, 0));

    /* Reed Switch Setup */
    if (!device_is_ready(reed_gpio.port)) {
        LOG_ERR("Reed switch GPIO not ready");
        return CHIP_ERROR_INCORRECT_STATE;
    }

    gpio_pin_configure_dt(&reed_gpio, GPIO_INPUT);
    gpio_init_callback(&reed_cb_data, ReedSwitchHandler, BIT(reed_gpio.pin));
    gpio_add_callback(reed_gpio.port, &reed_cb_data);
    gpio_pin_interrupt_configure_dt(&reed_gpio, GPIO_INT_EDGE_BOTH);

    /* Initial state */
    bool initial_state = gpio_pin_get_dt(&reed_gpio) == 0;
    auto *booleanState = Clusters::BooleanState::FindClusterOnEndpoint(kContactSensorEndpointId);
    if (booleanState) {
        booleanState->SetStateValue(initial_state);
    }

    ReturnErrorOnFailure(Nrf::Matter::StartServer());

    LOG_INF("Matter Contact Sensor ready - Reed on D0");
    return CHIP_NO_ERROR;
}

CHIP_ERROR AppTask::StartApp()
{
    ReturnErrorOnFailure(Init());

    while (true) {
        Nrf::DispatchNextTask();
    }

    return CHIP_NO_ERROR;
}
