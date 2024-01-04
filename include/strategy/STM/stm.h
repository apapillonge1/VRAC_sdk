#pragma once

#include "event.h"

//#include <fmtlog/fmtlog.h>
#include <fmt/color.h>
#include <range/v3/all.hpp>
#include <QObject>
#include <set>
#include <QDebug>

// TODO RAII Stm
template<typename state_t>
class Stm : public state_t
{

public:

    Stm(std::string name, state_t::context_type & ctx, state_t* initial_state, std::unordered_map<std::string, state_t*> && states) :
        state_t(name),
        states(states),
        entry_state(initial_state),
        ctx(ctx)
    {
        for (const auto & state : states) {
            state_t * s = state.second;
            QObject::connect(s, &state_t::send_event, this, &Stm::on_event, Qt::QueuedConnection);
        }
    }

    ~Stm() {
        for (auto * state : states | ranges::views::values) {
            delete state;
        }
    }
    std::string get_name(){
        return state_t::get_name();
    }

    void start() {
        Event e{"NoEvent"};
        on_entry(ctx,e);
        on_event(e);
    }

    void update() {}

    virtual void on_entry(state_t::context_type &, Event  e) override  {
        if (entry_state == nullptr) {
            return;
        }

        current_states.clear();
        current_states.push_back(entry_state);

        //FMTLOG(fmtlog::INF, "Entering state machine {} -> {}", state_t::name, entry_state);
        QDebug(QtDebugMsg) << QString("Entering state machine %1 -> %2").arg(QString::fromStdString(state_t::name)).arg(QString::fromStdString(entry_state->get_name()));

        entry_state->on_entry(ctx,e);
        state_t::send_event(Event{"NoEvent"});
    }

    virtual std::vector<std::string> on_event(Event e) override
    {
        const auto state_transition_map = current_states
                                          | ranges::views::transform([&](auto * state){
                                                return std::pair{state, state->on_event(e)};
                                            })
                                          | ranges::views::filter([](const auto & entry) {
                                                auto [_, target_states] = entry;
                                                return !std::empty(target_states);
                                            })
                                          | ranges::to<std::unordered_map<state_t*, std::vector<std::string>>>;

        // no transitions in sub states. checking as a state
        if (std::empty(state_transition_map)) {
            return state_t::on_event(e);
        }

        auto to_be_removed = state_transition_map
                             | ranges::views::keys
                             | ranges::to<std::vector>;

        auto to_be_added = state_transition_map
                           | ranges::views::values
                           | ranges::views::join
                           | ranges::views::transform([&](const auto & state_tag){
                                 return states.at(state_tag);
                             })
                           | ranges::to<std::vector>;

//        FMTLOG(fmtlog::INF, "{} {} {}",
//               fmt::format("{}",fmt::join(to_be_removed, "")),
//               fmt::format("--[{}]-->", e.value),
//               fmt::format("{}",fmt::join(to_be_added, ""))
//        );
        QDebug(QtDebugMsg) << QString("%1 %2 %3")
                                  .arg(QString::fromStdString(fmt::format("{}", fmt::join(to_be_removed, ""))))
                                  .arg(QString::fromStdString(fmt::format("--[{}]-->", e.value)))
                                  .arg(QString::fromStdString(fmt::format("{}", fmt::join(to_be_added, ""))));

        ranges::for_each(to_be_removed, [&](auto *state){
            state->on_exit(ctx, e);
            ranges::erase(current_states, ranges::remove(current_states, state), std::end(current_states));
        });
        ranges::for_each(to_be_added, [&](auto * state){
            state->on_entry(ctx, e);
        });

        current_states.insert(std::end(current_states), std::begin(to_be_added), std::end(to_be_added) );
        state_t::send_event(Event{"NoEvent"});
        return std::vector<std::string>{};
    }

    const auto &get_states() const { return states; }
    std::vector<state_t *> &get_current_states() { return current_states; }

private:
    std::unordered_map<std::string, state_t*> states;
    std::vector<state_t*> current_states;
    state_t* entry_state;
    state_t::context_type & ctx;
};
