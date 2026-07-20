struct Vehicle{};
struct ParkingSlot{
    string id;
    uint level;
    VehicleType type;
    pair<float,float> coords;
    float rate;
};
class SlotFinderStrategy{
    unordered_map<string,bool> slotStatus;
    set<Slot> sortedSlots;
    string findSlot(){
        
    }
};
class ParkingSlotManager{
    uint noOfFloors;
    vector<ParkingSlot> slots;
    vector<SlotFinderStrategy> slotFinders;
    Slot getSlot(VehicleType, uint gateNo, Preference pref){
        auto slotFinder = factory.get(pref, slotFinders);
        auto res = slotFinder.findSlot(VehicleType, gateNo);
        slots[slotID].status = BOOKED;
        for(auto& finder : slotFinders)
            finder.entry(slotId);
        return res;
    }
    void releaseSlot(string slotID){
        slots[slotID].status = FREE;
        for(auto& finder : slotFinders)
            finder.exit(slotId);
    }
};
struct EntryTicket{};
class ParkingManager{
    uint gateNo;
    EntryTicket makeEntry(uint gateNo){
        Vehicle vehicleInfo = scanVehicleInfo();
        return parkingManager.makeEntry(gateNo, vehicleInfo);
    }
    pair<ExitSlip,PaymentSlip> makeExit(gateNo){
        Vehicle vehicleInfo = scanVehicleInfo();
        EntryTicket ticket = bookings[vehicleInfo.regNo];
        float totalPrice = (currentTime() - ticket.entryTime) * ticket.rate;
        float finalPrice = pricingSrv.calculateFinalPrice(vehicleInfo.regNo, totalPrice);
        paymentSrv.makePayment(finalPrice);
        return parkingManager.makeExit(gateNo, vehicleInfo);
    }
};
class PaymentService{};
class PricingService{}; //manage coupons/discounts etc.
class ParkingApp{
    string name;
    uint noOfGates;
    vector<ParkingManager parkingManager;
    EntryTicket manageNewEntry(uint gateNo){
        return parkingManager.makeEntry(gateNo);
    }
    pair<ExitSlip,PaymentSlip> makeExit(gateNo){
        return parkingManager.makeExit(gateNo);
    }
};
