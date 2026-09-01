/*
Design the inventory service for an Amazon Now dark store. Support checkAvailability(sku, qty), 
reserve(sku, qty, orderId) (a temporary hold), confirm(orderId) (order placed), and release(orderId) (cart abandoned / payment failed). 
Stock must never be oversold.
*/

APIs:
CheckAvailabilty()
Reserve(qty,orderID)
releaseOrder(orderiD)
ConfirmOrder(orderID)

class Products{
    unordered_map<uint,ProductDetails> items;
};

class ReservedItem{
    uid,
    orderID;
    sku;
    qtty
    expiryTime
};

class 
class Inventory{
    unordered_map<uint,uint> ItemQtty;
    unordered_map<uint, unordered_map<uint,ReservedItem>> reservedItems; //orderID vs prodID vs qtty
    bool checkAvailability(uint productID, size_t qtty){
        return ItemQtty[productID] >= qtty;
    }
    bool reserve(orderID, sku, qtty){
            {
            lock(mtx);
            if(checkAvailability(sku, qtty) == false) return false;
            reservedItems[orderID][sku]+= qtty;
            //ItemQtty[sku]-= qtty;
            }
    }
    void handleExpiredreservedItems(){
        for(all reservedItem  : reservedItems)
            if(reservedItem.expired())
                deleteItem;
                ItemQtty[reserveditem.sku]+= reserveditem.qtty;
    }
    void confirm(orderID){
        for(all sku : reservedItems[orderID]
            ItemQtty[sku]--;
        reservedItems.erase(orderID);
    }
    void release(orderID){
        for(all sku : reservedItems[orderID]
            ItemQtty[sku]++;
        reservedItems.erase(orderID);        
    }
};

class Order{
    uint id;
    uint ord
}

class Orders{
    hasmap<uint, Order> orders;
}

class AmazonNow{
    Inventory inv;
    bool checkAvailability(productID, qtty){
        return inv.checkAvailiabilty(productID, qtty);    
    }
};
