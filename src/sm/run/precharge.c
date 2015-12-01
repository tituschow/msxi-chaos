#include "precharge.h"
#include "config.h"
#include "drivers/delay.h"
#include "events/motor_controller.h"
#include "events/fail.h"

// Motor controller precharge
// Init -> Precharge (Loop) -> End (-> Fail)

static void prv_init_sm(void);
static struct State init, precharge, end;
static struct StateMachine sm = {
  .default_state = &precharge,
  .init = prv_init_sm,
  .id = SM_PRECHARGE
};

static void prv_begin_precharge() {
  // Enable power to transducers
  mc_set_transducer_state(&mc_config, TRANSDUCER_ENABLED);

  // Allow the charge pump and transducers to power up?
  mc_process(&mc_config, mc_precharge_begin, PRECHARGE_BEGIN, PRECHARGE_FAIL);
}

static void prv_check_precharge() {
  delay_seconds(1); // TODO: tune delay
  mc_process(&mc_config, mc_precharge_power, PRECHARGE_SUCCESS, PRECHARGE_TIMEOUT);
}

static void prv_end_precharge() {
  // Disable +/-12V to transducers no matter what happens
  mc_set_transducer_state(&mc_config, TRANSDUCER_DISABLED);

  mc_process(&mc_config, mc_precharge_end, PRECHARGE_COMPLETE, PRECHARGE_FAIL);
}

static void prv_handle_fail(struct StateMachine *sm, uint64_t mc) {
  // Disable +/-12V to transducers
  mc_set_transducer_state(&mc_config, TRANSDUCER_DISABLED);

  // Raise generic fail event
  fail_handle_mc(sm, mc);
}

static void prv_init_sm() {
  state_init(&init, prv_begin_precharge);
  state_add_state_transition(&init, PRECHARGE_BEGIN, &precharge);
  state_add_transition(&init, transitions_make_event_data_rule(PRECHARGE_FAIL, NO_GUARD,
                                                               prv_handle_fail));

  state_init(&precharge, prv_check_precharge);
  state_add_state_transition(&init, PRECHARGE_TIMEOUT, &precharge); // Loop
  state_add_state_transition(&init, PRECHARGE_SUCCESS, &end);

  state_init(&end, prv_end_precharge);
  state_add_transition(&init, transitions_make_event_data_rule(PRECHARGE_FAIL, NO_GUARD,
                                                               prv_handle_fail));
}

struct StateMachine *precharge_get_sm(void) {
  return &sm;
}

EventID precharge_get_exit_event(void) {
  return PRECHARGE_COMPLETE;
}
