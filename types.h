
struct ClientTable {
    char cClientID;
    char cSelfID;
    int iSocketFD;
};

struct IDHeader {
    char selfID;
    char clientID;
};