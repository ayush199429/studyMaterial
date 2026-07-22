/*
Design a category hierarchy system, where a category can have subcategories and products. Add a method to get all products with given filters (for ex. Category, name etc). System should be extensible for filter adding in the future.
*/

class Product{
    string name, model, srNo;
    unordered_map<string,string> details;
};
class CategoryHeirarchy{
    unordered_map<string,vector<string>> heirarchyOfItems;
    void sort(Product p){
        for(string cat : heirarchyOfItems[p.baseItem]){
            if(p.hasKey(cat) == false)
                p.addKey(cat,"NA");
        }
    }
    string getNextCategory(string item, string cur){
        
    }
};
class Category{
    Category(){
        nextCategory = CategoryHeirarchy::getNextCategory(baseItem, categoryName);
    }
    string baseItem;
    string categoryName, value, nextCategory;
    unordered_map<string, Category*> subCategories;
    vector<Product> products;
    void addProduct(Product product){
        if(nextCategory == ""){
            products.push_back(product);
            return;
        }
        if(subCategories.find(value) == subCategories.end())
            subCategories[value] = new Category(baseItem, nextCategory, value);
        return subCategories[value]->addProduct(product);
    }
    vector<Product> search(Product product){
        
    }
};
class CategorySrv{
    unordered_map<string,Category*> rootOfItems;
    void add(Product p){
        Product modifiedProduct = CategoryHeirarchy::sort(product);
        rootOfItems[baseItem]->add(modifiedProduct);
    }
    vector<Product> search(Product p){
        Product modifiedProduct = CategoryHeirarchy::sort(p);
        return rootOfItems[baseItem]->search(modifiedProduct);        
    }
};
