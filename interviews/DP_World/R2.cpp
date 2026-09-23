n*n matrix having 1 to n2 numbers. One num is missing and one is repeated. Find both.
- I solved using marking the index for every number as negative and if we encounter a negative num twice then that is the repeated number.

LLD:
Port{
    portID,
    name,
    location,
    list<StorageArea> stores;
    StoreFactory findStoreFactory;
    ReturnStatus unloadContainer(Container c){
        unloadingMachine.unload(c.getBoxNumber());
        uint slotID = getSlotNumberToPlaceTheConatiner(c);
        placingMachine.placeContainer(slotID);
    }
    void getSlotNumberToPlaceTheConatiner(Conatiner c){
        ContainerType contType = c.getType();
        uint storeId = findStoreFactory.find(contType);
        store = stores[storeID];
        return store.getSlotNumber(c);
    }
};
enum ShipStage{
    IMPORTING_TO_PORT,
    AT_PORT,
    UNLOADING,
    LOADING,
    EXPORTING_BACK
};
enum ContainerStage{
    IN_TRUCK,
    WAITING_FOR_SHIP,
    IN_SHIP,
    AT_PORT,
    WAITING_AT_STORAGE_AREA,
};
Ship{
    id,
    ShipStage currStage;
    location currLocation;
    list<Container> containers;
    list<Box> boxes;
    dstPort
};
Container{
    id,
    ContainerSize size,
    uint shipID,
    ContainerStage curStage;
    uint boxNumInsideShip;
    //uint lastShipID,
};
StorageArea{
    id,
    location,
    list<Slot>
    uint findSlot(Container c){
        
    }
    bool getSlotNumber(Container c){
        uint slotID = findSlot(c);
        return slotID;
    }
};
enum SlotStatus{
    FREE,
    FILLED,
    UNDER_MAINTAINANCE
};
Slot{
    id,
    SlotType type
    SlotStatus status
};

HLD:
if an API is sometimes slow, sometimes fine, then what could be the causes?
- I told DB, downstream API, CPU overloaded.

