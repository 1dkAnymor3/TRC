#include <dpp/dpp.h>
#include <string>

using namespace std;

int main() {
    /* Setup the bot */
    dpp::cluster bot("bro i aint putting my id publicly", dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    /* Message handler to look for a command called !button */
    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (event.msg.content == "!button") {
            /* Create a message containing an action row, and a button within the action row. */
            bot.message_create(
                dpp::message(event.msg.channel_id, "this text has buttons").add_component(
                    dpp::component().add_component(
                        dpp::component().set_label("Click me!").
                        set_type(dpp::cot_button).
                        set_emoji(u8"😄").
                        set_style(dpp::cos_danger).
                        set_id("myid")
                    )
                )
            );
        }
        else if (event.msg.content == "!listemojiid") {
            string string_request = "";
            char piece_name[6] = { "r", "n", "b", "q", "k", "p" };
            int piece_type = 0;
            int piece_square = 0;
            for (char piece: piece_name) {
                for (piece_type = 0; piece_type < 2; piece_type++) {
                    for (piece_square = 0; piece_square < 2; piece_square++) {
                        if (piece_type == 0) {
                            if (piece_square == 0) {
                                string_request = string_request + to_string(piece) + "ll";
                            }
                            else {
                                string_request = string_request + to_string(piece) + "ld";
                            }
                        }
                        else {
                            if (piece_square == 0) {
                                string_request = string_request + to_string(piece) + "dl";
                            }
                            else {
                                string_request = string_request + to_string(piece) + "dd";
                            }
                        }
                    }
                }
            }
            dpp::message(event.msg.channel_id, string_request);
        }
        });

    /* When a user clicks your button, the on_button_click event will fire,
     * containing the custom_id you defined in your button.
     */
    bot.on_button_click([&bot](const dpp::button_click_t& event) {
        /* Button clicks are still interactions, and must be replied to in some form to
         * prevent the "this interaction has failed" message from Discord to the user.
         */
        event.reply("You clicked: " + event.custom_id);
        });

    bot.start(dpp::st_wait);

    return 0;
}
