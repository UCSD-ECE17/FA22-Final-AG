//
// Created by Vikash Gilja on 12/5/22.
//

#include "tests.hpp"

#include "IceCream.hpp"
#include "IceCreamTopper.hpp"
#include "IceCreamMaker.hpp"
#include "functions.hpp"
#include "PriceSheetV2.hpp"
#include "Inventory.hpp"

void test_result(size_t &points, size_t value, bool passed, std::string testName) {
    std::cout << "\t" << testName << " (" << value << " point(s)) : ";
    if(passed) {
        std::cout << "PASSED\n";
        points += value;
    }
    else std::cout << "FAILED\n";
}

size_t basic_ice_cream_test() {
    size_t points = 0;
    bool result;

    std::vector<IceCreamConcrete> iceCreamListVector = basic_ice_cream();
    result = 4 == iceCreamListVector.size();

    // PART1: TEST A
    test_result(points, 1, result, "(A) List has 4 items");
    if (!result) {
        std::cout << "\tExiting Part 1 testing early, need to pass (A)!" << std::endl;
        return points;
    }

    // PART1: Test B-E
    test_result(points, 1, iceCreamListVector[0].describeIceCream() == "A cup with 1 scoop(s) of vanilla ice cream", "(B) 1st item is correct");
    test_result(points, 1, iceCreamListVector[1].describeIceCream() == "A cone with 1 scoop(s) of vanilla ice cream", "(C) 2nd item is correct");
    test_result(points, 1, iceCreamListVector[2].describeIceCream() == "A cup with 1 scoop(s) of strawberry ice cream", "(D) 3rd item is correct");
    test_result(points, 1, iceCreamListVector[3].describeIceCream() == "A cone with 3 scoop(s) of chocolate ice cream", "(E) 4th item is correct");

    return points;
}

size_t add_toppings_test() {
    size_t points = 0;
    bool result;

    IceCreamConcrete* iceCream = new IceCreamConcrete;
    std::cout << add_sprinkles(iceCream)->describeIceCream() << std::endl;
    result = add_sprinkles(iceCream)->describeIceCream() == "A cup with 1 scoop(s) of vanilla ice cream with sprinkles";
    test_result(points, 2, result, "(A) Sprinkles were added");

    IceCreamConcrete* iceCreamSundae = new IceCreamConcrete;

    result = construct_a_sundae(iceCream)->describeIceCream() == "A cup with 1 scoop(s) of vanilla ice cream with whip cream with fudge with sprinkles with a cherry";
    test_result(points, 3, result, "(B) Sundae was constructed");

    delete iceCream;
    delete iceCreamSundae;

    return points;
}

size_t toppings_cost_test() {
    size_t points = 0;
    bool result;

    Inventory inventory;
    inventory.setItemsAvailable(Flavor::VANILLA, 100);
    inventory.setItemsAvailable(Container::CUP, 100);
    inventory.setItemsAvailable(Topping::SPRINKLES, 100);
    inventory.setItemsAvailable(Topping::CHERRY, 100);

    IceCreamMaker iceCreamMaker;
    iceCreamMaker.setInventory(&inventory);

    PriceSheet priceSheet;
    IceCream* ic = iceCreamMaker.make();
    result = 1.25 == ic->price(priceSheet);
    test_result(points, 1, result, "(A) Basic Ice Cream Pricing");


    iceCreamMaker.addTopping(Topping::CHERRY);
    IceCream* ic2 = iceCreamMaker.make();
    result = 1.75 == ic2->price(priceSheet);
    test_result(points, 3, result, "(B) Ice Cream Pricing with Topping");

    iceCreamMaker.addTopping(Topping::SPRINKLES);
    IceCream * ic3 = iceCreamMaker.make();
    result = 2.25 == ic3->price(priceSheet);
    test_result(points, 2, result, "(C) Ice Cream Pricing with a Cherry and Sprinkles");

    PriceSheetV2 priceSheetV2;
    result = 1.50 == ic->price(priceSheetV2);
    test_result(points, 4, result, "(D) Updated Basic Ice Cream Pricing");

    result = 2 == ic2->price(priceSheetV2);
    test_result(points, 2, result, "(E) Updated Ice Cream Pricing with Cherry Topping");

    result = 2.75 == ic3->price(priceSheetV2);
    test_result(points, 1, result, "(F) Updated Ice Cream Pricing with a Cherry and Sprinkles");

    delete ic;
    delete ic2;
    delete ic3;
    return points;
}

size_t visitor_tests() {
    size_t points = 0;
    bool result;

    Inventory inventory;
    inventory.setItemsAvailable(Flavor::VANILLA, 100);
    inventory.setItemsAvailable(Flavor::CHOCOLATE, 100);
    inventory.setItemsAvailable(Container::CUP, 100);
    inventory.setItemsAvailable(Topping::SPRINKLES, 100);
    inventory.setItemsAvailable(Topping::CHERRY, 100);

    IceCreamMaker iceCreamMaker;
    iceCreamMaker.setInventory(&inventory);

    Ledger ledger;

    ledger.addOrder(iceCreamMaker.make(), PriceSheet());
    ledger.addOrder(iceCreamMaker.make(), PriceSheet());
    iceCreamMaker.setFlavor(Flavor::CHOCOLATE);
    iceCreamMaker.setScoopCount(3);
    ledger.addOrder(iceCreamMaker.make(), PriceSheet());

    result = 3 == ledger.visit(ledgerCount);
    test_result(points, 3, result, "(A) The visitor counted ledger entries correctly (Test 1)");

    ledger.addOrder(iceCreamMaker.make(), PriceSheet());
    ledger.addOrder(iceCreamMaker.make(), PriceSheet());

    result = 5 == ledger.visit(ledgerCount);
    test_result(points, 3, result, "(B) The visitor counted ledger entries correctly (Test 2)");

    result = 10.75 == ledgerTotal(ledger);
    test_result(points, 4, result, "(C) The ledger total is correct (Test 1)");
    iceCreamMaker.setScoopCount(1);
    ledger.addOrder(iceCreamMaker.make(), PriceSheet());
    ledger.addOrder(iceCreamMaker.make(), PriceSheet());
    result = 13.25 == ledgerTotal(ledger);
    test_result(points, 4, result, "(D) The ledger total is correct (Test 2)");

    return points;
}

size_t inventory_check_test() {
    size_t points = 0;
    bool result;

    IceCreamMaker iceCreamMaker;
    result = nullptr == iceCreamMaker.make();
    test_result(points, 1, result, "(A) When no inventory is set, nullptr is returned");

    Inventory inventory;
    inventory.setItemsAvailable(Flavor::VANILLA, 0);
    inventory.setItemsAvailable(Container::CUP, 1);
    inventory.setItemsAvailable(Topping::SPRINKLES, 2);
    iceCreamMaker.setInventory(&inventory);
    iceCreamMaker.addTopping(Topping::SPRINKLES);
    iceCreamMaker.addTopping(Topping::SPRINKLES);

    result = nullptr == iceCreamMaker.make();
    inventory.setItemsAvailable(Flavor::VANILLA, 1);
    inventory.setItemsAvailable(Container::CUP, 0);
    result = result && (nullptr == iceCreamMaker.make());

    inventory.setItemsAvailable(Flavor::VANILLA, 1);
    inventory.setItemsAvailable(Container::CUP, 1);
    iceCreamMaker.addTopping(Topping::SPRINKLES);

    result = result && (nullptr == iceCreamMaker.make());

    test_result(points, 2, result, "(B) When inventory is missing a requested item, nullptr is returned");

    inventory.setItemsAvailable(Flavor::VANILLA, 100);
    inventory.setItemsAvailable(Flavor::CHOCOLATE, 100);
    inventory.setItemsAvailable(Container::CUP, 100);
    inventory.setItemsAvailable(Topping::SPRINKLES, 100);
    inventory.setItemsAvailable(Topping::CHERRY, 100);

    iceCreamMaker.clearToppings();
    iceCreamMaker.setScoopCount(3);
    iceCreamMaker.make();
    iceCreamMaker.make();

    result = (94 == inventory.getItemsAvailable(Flavor::VANILLA))
            && (98 == inventory.getItemsAvailable(Container::CUP))
            && (100 == inventory.getItemsAvailable(Flavor::CHOCOLATE))
            && (100 == inventory.getItemsAvailable(Topping::CHERRY))
            && (100 == inventory.getItemsAvailable(Topping::SPRINKLES));
    test_result(points, 3, result, "(C) Inventory has been updated correctly after making 2 cups with 3 vanilla scoops each");

    iceCreamMaker.setScoopCount(1);
    iceCreamMaker.addTopping(Topping::SPRINKLES);
    iceCreamMaker.addTopping(Topping::SPRINKLES);
    iceCreamMaker.addTopping(Topping::CHERRY);
    iceCreamMaker.make();
    iceCreamMaker.make();
    iceCreamMaker.make();
    iceCreamMaker.make();

    result = (90 == inventory.getItemsAvailable(Flavor::VANILLA))
             && (94 == inventory.getItemsAvailable(Container::CUP))
             && (100 == inventory.getItemsAvailable(Flavor::CHOCOLATE))
             && (96 == inventory.getItemsAvailable(Topping::CHERRY))
             && (92 == inventory.getItemsAvailable(Topping::SPRINKLES));
    test_result(points, 3, result, "(D) Inventory has been updated correctly after making 2 cups with 3 vanilla scoops each & toppings");

    iceCreamMaker.clearToppings();
    size_t iceCreamsMade = 0;
    while (iceCreamsMade < 100 && nullptr != iceCreamMaker.make()) {
        iceCreamsMade++;
    }
    result = 90 == iceCreamsMade;
    test_result(points, 3, result, "(E) nullptr returned from IceCreamMaker once vanilla is used up.");

    iceCreamMaker.setFlavor(Flavor::CHOCOLATE);
    iceCreamsMade = 0;
    while (iceCreamsMade < 100 && nullptr != iceCreamMaker.make()) {
        iceCreamsMade++;
    }
    result = 4 == iceCreamsMade;
    test_result(points, 3, result, "(F) nullptr returned from IceCreamMaker once cups are all used.");

    inventory.setItemsAvailable(Flavor::VANILLA, 100);
    inventory.setItemsAvailable(Flavor::CHOCOLATE, 100);
    inventory.setItemsAvailable(Container::CUP, 100);
    inventory.setItemsAvailable(Topping::SPRINKLES, 10);
    inventory.setItemsAvailable(Topping::CHERRY, 10);
    iceCreamMaker.addTopping(Topping::SPRINKLES);
    iceCreamMaker.addTopping(Topping::CHERRY);
    iceCreamMaker.addTopping(Topping::SPRINKLES);

    iceCreamsMade = 0;
    while (iceCreamsMade < 100 && nullptr != iceCreamMaker.make()) {
        iceCreamsMade++;
    }
    result = 5 == iceCreamsMade;
    test_result(points, 3, result, "(G) nullptr returned from IceCreamMaker once toppings are all used.");


    return points;
}

