#include "utils/NX.hpp"
#include <algorithm>
#include <iterator>

// Maximum number of titles to read using pdm
#define MAX_TITLES 4096

// Comparison of AccountUids
bool operator == (const AccountUid &a, const AccountUid &b) {
    if (a.uid[0] == b.uid[0] && a.uid[1] == b.uid[1]) {
        return true;
    }
    return false;
}

namespace Utils::NX {
    ThemeType getHorizonTheme() {
        ColorSetId thm;
        Result rc = setsysGetColorSetId(&thm);
        if (R_SUCCEEDED(rc)) {
            switch (thm) {
                case ColorSetId_Light:
                    return ThemeType::Light;
                    break;

                case ColorSetId_Dark:
                    return ThemeType::Dark;
                    break;
            }
        }

        // If it fails return dark
        return ThemeType::Dark;
    }

    Language getSystemLanguage() {
        SetLanguage sl;
        u64 l;
        setInitialize();
        setGetSystemLanguage(&l);
        setMakeLanguage(l, &sl);
        setExit();

        Language lang;
        switch (sl) {
            case SetLanguage_ENGB:
            case SetLanguage_ENUS:
                lang = English;
                break;

            case SetLanguage_FR:
                lang = French;
                break;

            case SetLanguage_DE:
                lang = German;
                break;

            case SetLanguage_IT:
                lang = Italian;
                break;

            case SetLanguage_PT:
                lang = Portugese;
                break;

            case SetLanguage_RU:
                lang = Russian;
                break;

            case SetLanguage_ES:
                lang = Spanish;
                break;

            case SetLanguage_ZHHANT:
                lang = ChineseTraditional;
                break;

            case SetLanguage_ZHCN:
            case SetLanguage_ZHHANS:
                lang = Chinese;
                break;

            case SetLanguage_KO:
                lang = Korean;
                break;

            default:
                lang = Default;
                break;
        }

        return lang;
    }

    ::NX::User * getUserPageUser() {
        ::NX::User * u = nullptr;

        AppletType t = appletGetAppletType();
        if (t == AppletType_LibraryApplet) {
            // Attempt to get user id from IStorage
            AppletStorage * s = (AppletStorage *)malloc(sizeof(AppletStorage));
            // Pop common args IStorage
            if (R_SUCCEEDED(appletPopInData(s))) {
                // Pop MyPage-specific args IStorage
                if (R_SUCCEEDED(appletPopInData(s))) {
                    // Get user id
                    AccountUid uid;
                    appletStorageRead(s, 0x8, &uid, 0x10);

                    // Check if valid
                    AccountUid userIDs[ACC_USER_LIST_SIZE];
                    s32 num = 0;
                    accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);
                    for (s32 i = 0; i < num; i++) {
                        if (uid == userIDs[i]) {
                            u = new ::NX::User(uid);
                            break;
                        }
                    }
                }
            }
            free(s);
        }

        return u;
    }

    std::vector<::NX::User *> getUserObjects() {
        // Get IDs
        std::vector<::NX::User *> users;
        AccountUid userIDs[ACC_USER_LIST_SIZE];
        s32 num = 0;
        Result rc = accountListAllUsers(userIDs, ACC_USER_LIST_SIZE, &num);

        if (R_SUCCEEDED(rc)) {
            // Create objects and insert into vector
            for (s32 i = 0; i < num; i++) {
                users.push_back(new ::NX::User(userIDs[i]));
            }
        }

        // Returns an empty vector if an error occurred
        return users;
    }

    std::vector<::NX::Title *> getTitleObjects(std::vector<::NX::User *> u) {
        Result rc = 0;
        // Get ALL played titles for ALL users
        // (this doesn't include installed games that haven't been played)
        std::vector<TitleID> playedIDs;
        for (auto user : u) {
            // Position of first event to read
            s32 offset = 0;
            // Total events read in iteration
            s32 playedTotal = 1;

            // Array to store read events
            PdmAccountPlayEvent *userPlayEvents  = new PdmAccountPlayEvent[MAX_TITLES];

            TitleID tmpID = 0;
            // Read all events
            while (playedTotal > 0) {
                memset(userPlayEvents , 0, MAX_TITLES * sizeof(PdmAccountPlayEvent));
                rc = pdmqryQueryAccountPlayEvent(offset, user->ID(), userPlayEvents , MAX_TITLES, &playedTotal);
                if (R_SUCCEEDED(rc)) {
                    // Set next read position to next event
                    offset += playedTotal;

                    // Push back ID if not already in the vector
                    for (s32 i = 0; i < playedTotal; i++) {
                        tmpID = (static_cast<TitleID>(userPlayEvents[i].application_id[0]) << 32) | userPlayEvents[i].application_id[1];
                        if (std::find_if(playedIDs.begin(), playedIDs.end(), [tmpID](auto id){ return (id == tmpID); }) == playedIDs.end()) {
                            if (tmpID != 0) {
                                playedIDs.push_back(tmpID);
                            }
                        }
                    }
                }
            }

            // Free memory allocated to array
            delete[] userPlayEvents ;
        }

        // Get IDs of all installed titles
        std::vector<TitleID> installedIDs;
        NsApplicationRecord *records = new NsApplicationRecord[MAX_TITLES];
        s32 count = 0;
        s32 out = 0;
        while (true) {
            memset(records, 0, MAX_TITLES * sizeof(NsApplicationRecord));
            rc = nsListApplicationRecord(records, MAX_TITLES, count, &out);
            // Break if at the end or no titles
            if (R_FAILED(rc) || out == 0){
                break;
            }
            for (s32 i = 0; i < out; i++) {
                if ((records + i)->application_id != 0) {
                    installedIDs.push_back((records + i)->application_id);
                }
            }
            count += out;
        }
        delete[] records;

        // Create Title objects from IDs
        std::vector<::NX::Title *> titles;
        for (auto playedID : playedIDs) {
            // Loop over installed titles to determine if installed or not
            bool installed = std::find_if(installedIDs.begin(), installedIDs.end(), [playedID](auto id) { return id == playedID; }) != installedIDs.end();
            titles.push_back(new ::NX::Title(playedID, installed));
        }

        return titles;
    }

    void startServices() {
        accountInitialize(AccountServiceType_System);
        nsInitialize();
        pdmqryInitialize();
        romfsInit();
        setsysInitialize();
        socketInitializeDefault();

        #if _NXLINK_
            nxlinkStdio();
        #endif
    }

    void stopServices() {
        accountExit();
        nsExit();
        pdmqryExit();
        romfsExit();
        setsysExit();
        socketExit();
    }
};
