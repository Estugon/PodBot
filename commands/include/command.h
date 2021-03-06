#ifndef COMMANDS_H
#define COMMANDS_H


#include <iostream>
#include <map>
#include "sleepy_discord/sleepy_discord.h"
#include "myAnimeList.h"
#include "lastfmAPI.h"
#include "json.hpp"

/**
 * A generic command.
 */
class Command {
    public:
        Command();
        virtual SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        virtual std::string description();
        virtual std::string longDescription();
        virtual std::vector<std::string> parameterize(std::string);
        virtual bool isSecret();
};

/**
 * Helpflul help command. 
 * Will return a message with either a general overview over every possible command,
 * or a help message for a specified command.
 */
class Help : public Command {
    public:
        Help(std::map<std::string, Command*>);
        SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        bool isSecret();
    private:
        static std::map<std::string, Command*> availableCommands;
        
};

/**
 * A command for owofieing strings.
 */
class OwOfier : public Command {
public:
    OwOfier();
    SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
    std::string description();
    std::string longDescription();
private:
    std::string owofy(std::string);
};

/**
 * Commands for interacting with the MyAnimeList Database.
 * If successfull, returns a message with information about 
 * an anime/a mange/whatever the database has to offer.
 */
class MyAnimeListCommands : public Command {
    public:
        MyAnimeListCommands();
        SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        SleepyDiscord::Embed createEmbed(Anime);
        std::string description();
        std::string longDescription();
};

/**
 * Commands for interacting with the LastFM Database.
 * If successfull, returns a message with information about 
 * a song/an album/whatever the database has to offer.
 */
class LastFMCommand : public Command {
    public:
        LastFMCommand();
        SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        std::string description();
        std::string longDescription();
    private:
        SleepyDiscord::Embed createEmbed(lastfm::Song);
        SleepyDiscord::Embed createEmbed(lastfm::Artist);
        SleepyDiscord::Embed createEmbed(lastfm::Album);
        SleepyDiscord::Embed createEmbed(lastfm::User);
};

class Kohaku : public Command {
    public:
        Kohaku();
        SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        bool isSecret();
        std::string longDescription();
};

class Pat : public Command {
    public:
        Pat(SleepyDiscord::DiscordClient*);
        SleepyDiscord::SendMessageParams execute(std::string, SleepyDiscord::Message*);
        std::string description();
        std::string longDescription();
    private:
        static nlohmann::json patData; 
        static SleepyDiscord::DiscordClient* client;

        /**
         * Headpats a specific user and updates the json database.
         * @param source: The user who is headpatting someone.
         * @param target: The user who is getting headpatted.
         */
        void pat(std::string, std::string);

        /**
         * Get the number of times a user was headpatted.
         * @param user: The ID of the user.
         */
        int getPatReceivedCount(std::string);

        /**
         * Get the number of times a user has headpatted someone.
         * @param user: The ID of the user.
         */
        int getPatGivenCount(std::string);
        bool stringBeginsWith(std::string, std::string); //TODO: Put this in another helper headerfile, you dumbass

        /**
         * Loads the saved json data from disk into memory.
         */
        void loadData();
        /**
         * Write the json database to the harddrive.
         */
        void saveData();

        /**
         * Try to find a user on a server via it's nickname
         * @param username: The users name.
         * @param serverID: The ID of the server the user is on.
         */
        bool findUserID(std::string, std::string serverID, std::vector<std::pair<std::string, std::string>>*);

        /**
         * Extract a command from a string.
         * (Possible commands: gcount, rcount)
         * @param param: The string that may hold the command.
         * @return rcount or gcount if a command was found, if not an empty string
         */
        std::string getCommand(std::string);

        /**
         * Get the users mentioned in a string.
         * If no user was found, users will only receive {"pod", "pod"} as a entry.
         * @param param: The input string
         * @param serverID: The Discord server ID of the server the user(s) should be on.
         * @param users: A vector of pairs with the name of the user in the first entry and thd ID of the user in the second entry.
         *               Will be filled by this method.
         */
        void getUsers(std::string, std::string, std::vector<std::pair<std::string, std::string>>*);

        /**
         * Pats users! 
         * Will return a string with a message for the discord chat.
         * @param user: The user who is patting people
         * @param users: The users who will be patted, in a vector of <string USERNAME, string USER_ID> pairs.
         * @return: A string with a message informing the user who was patted, and who was not patted (or other events).
         */
        std::string executePat(std::string, std::vector<std::pair<std::string, std::string>>*);

        /**
         * Executes a command (rcount, gcount).
         * Is anyone even reading these comments? 
         * @param command: The command. Duh.
         * @param user: The needed by the command. May be empty.
         * @return: The output of the command.
         */
        std::string executeCommand(std::string, std::pair<std::string, std::string>);
        /**
         * Since we don't want the cache to be refreshed too often,
         * only refresh it when this variable is true.
         */
        static bool mayRefreshCache;
        /**
         * A map structure with server IDs as the keys and jsons with server members 
         * and their corresponding IDs,Names, Nicks as values.
         */
        static std::map<std::string, nlohmann::json> membersOnServer;
};


#endif // !COMMANDS_H