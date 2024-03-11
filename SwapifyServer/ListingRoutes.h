#pragma once

#include "TCPServer.h"
#include "Route.h"
#include "SQLiteDatabase.h"
#include "Listing.h"


class ListingRoutes : public Route {
public:
    Packet* handleRequest(Packet& packet, SOCKET clientSocket, std::string action) override;

private:
    Packet* handleCreateListing(Listing listing);
    Packet* handleGetListing(Listing listing);
    Packet* handleUpdateListing(Listing listing);
    Packet* handleDeleteListing(Listing listing);
};