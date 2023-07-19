#include <dpp/dpp.h>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <fstream>

std::vector<std::string> allQuotes = { "placeholder", "another placeholder" };
std::string quote;
const std::string dash = " - ";
std::string quoteUserID;

bool findWord(std::string sentence, std::string word)
{
    std::istringstream iss(sentence);
    std::vector<std::string> split;
    std::copy(std::istream_iterator<std::string>(iss),
        std::istream_iterator<std::string>(),
        std::back_inserter(split));
    for (auto i = split.begin(); i != split.end(); i++){
        if (*i == word){
            return true;
        }
    }
    return false;
}

std::string replace(std::string s, char find, char replace)
{
    for (int i = 0; i < s.length(); i++)
    {
        if (s[i] == find) {
            s[i] = replace;
        }
    }
    return s;
}

void importQuotes(std::vector<std::string>& allQuotes)
{
    std::ifstream fin("AllQuotes.txt");

    allQuotes.clear();

    std::string element;
    while (fin >> element)
    {
        allQuotes.push_back(element);
    }
    for (auto i = allQuotes.begin(); i != allQuotes.end(); i++)
    {
        *i = replace(*i, '_', ' ');
    }
}

int main() {
    srand(time(nullptr));
    dpp::cluster bot("bot token", dpp::i_default_intents | dpp::i_message_content);

    bot.on_log(dpp::utility::cout_logger());

    importQuotes(allQuotes);

    bot.on_message_create([&bot](const dpp::message_create_t& event) {
        if (findWord(event.msg.content, "cunny")) {
            bot.message_create(
                dpp::message(event.msg.channel_id, "too much kaiser").set_reference(event.msg.id)
            );
        }
        else if (event.msg.content == "&rollDice")
        {
            int num = rand() % 6 + 1;
            bot.message_create(
                dpp::message(event.msg.channel_id, std::to_string(num)).set_reference(event.msg.id)
            );
        }
        else if (event.msg.content == "&quote")
        {
            if (allQuotes.size() == 0)
            {
                bot.message_create(
                    dpp::message(event.msg.channel_id, "I've got none! - Bot").set_reference(event.msg.id)
                );
            }
            else
            {
                bot.message_create(dpp::message(event.msg.channel_id, allQuotes.at((rand() * 100) % allQuotes.size())).set_reference(event.msg.id));
            }
        }
        else if (event.msg.content == "&help")
        {
            dpp::embed embed = dpp::embed().
                set_color(dpp::colors::sti_blue).
                set_title("All Commands").
                set_url("https://discord.com/api/oauth2/authorize?client_id=1102420340837191740&permissions=309237766144&scope=bot").
                set_author("1mnot0k", "https://blank.page/", "https://cdn.discordapp.com/avatars/1072132964252651570/03f0dd816a82b15ded24299add62e84d.webp").
                set_description("A list of all commands").
                set_thumbnail("https://cdn.discordapp.com/attachments/1116299358598615103/1131063680298127380/what.png").
                add_field(
                    "&help",
                    "List all commands"
                ).
                add_field(
                    "&rollDice",
                    "Generate a number between 1 to 6",
                    true
                ).
                add_field(
                    "&quote",
                    "Gives a message that was sent in the server",
                    true
                ).
                set_image("https://cdn.discordapp.com/attachments/1116299358598615103/1131063680298127380/what.png").
                set_footer(dpp::embed_footer().set_text("Testing bot").set_icon("https://cdn.discordapp.com/attachments/1116299358598615103/1131063680298127380/what.png")).
                set_timestamp(time(0));

            bot.message_create(dpp::message(event.msg.channel_id, embed).set_reference(event.msg.id));
        }
        else
        {
            if (event.msg.author.get_mention() != "<@ application id>")
            {
                quote = event.msg.content;
                quoteUserID = event.msg.author.get_mention();

                quote.append(dash);
                quote.append(quoteUserID);
                std::cout << quote << std::endl;
                allQuotes.push_back(quote);
                if (allQuotes.size() > 100)
                {
                    allQuotes.erase(allQuotes.begin());
                }
            }
        }
        });

    bot.start(dpp::st_wait);

    return 0;
}
