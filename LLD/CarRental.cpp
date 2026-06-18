enum class VehicleType
{
    SEDAN,
    SUV,
    HATCHBACK,
    LUXURY
};

enum class VehicleStatus
{
    AVAILABLE,
    MAINTENANCE,
    RETIRED
};

enum class BookingStatus
{
    PENDING_PAYMENT,
    CONFIRMED,
    PAYMENT_FAILED,
    CANCELLED,
    COMPLETED
};

class User
{
public:
    uint userId;
    std::string name;
};

class Vehicle
{
public:
    uint vehicleId;
    VehicleType type;
    VehicleStatus status;
    std::string city;
};

class Booking
{
public:
    uint bookingId;

    uint userId;
    uint vehicleId;

    Date startDate;
    Date endDate;

    BookingStatus status;

    double amount;
};

struct BookingComparator
{
    bool operator()(const Booking& a,
                    const Booking& b) const
    {
        return a.startDate < b.startDate;
    }
};

class UserService
{
private:
    std::unordered_map<uint, User> users;

public:
    void addUser(const User& user);

    User* getUser(uint userId);
};

class VehicleService
{
private:
    std::unordered_map<uint, Vehicle> vehicles;

public:
    void addVehicle(const Vehicle& vehicle);

    Vehicle* getVehicle(uint vehicleId);

    std::vector<Vehicle> searchVehicles(
        const std::string& city,
        VehicleType type);
};

class PricingService
{
public:
    double calculatePrice(
        const Vehicle& vehicle,
        Date startDate,
        Date endDate);

    double calculateRefund(
        const Booking& booking);
};

class PaymentService
{
public:
    bool makePayment(
        uint userId,
        double amount);

    bool refund(
        uint userId,
        double amount);
};

class BookingService
{
private:

    // bookingId -> Booking
    std::unordered_map<uint, Booking> bookingMap;

    // userId -> bookingIds
    std::unordered_map<uint,
                       std::vector<uint>> userBookings;

    // vehicleId -> bookings sorted by startDate
    std::unordered_map<
        uint,
        std::set<Booking, BookingComparator>
    > vehicleBookings;

    std::unordered_map<uint, std::mutex> vehicleLocks;

    PaymentService paymentSrv;
    PricingService pricingSrv;

public:

    bool checkAvailability(
        uint vehicleId,
        Date startDate,
        Date endDate);

    Booking createBooking(
        uint userId,
        uint vehicleId,
        Date startDate,
        Date endDate);

    bool cancelBooking(
        uint bookingId);

    void onPaymentSuccess(
        uint bookingId);

    void onPaymentFailed(
        uint bookingId);
};

class CarRentalSystem
{
private:
    UserService userSrv;
    VehicleService vehicleSrv;
    BookingService bookingSrv;

public:

    std::vector<Vehicle> searchVehicles(
        const std::string& city,
        VehicleType type)
    {
        return vehicleSrv.searchVehicles(
            city,
            type);
    }

    Booking bookVehicle(
        uint userId,
        uint vehicleId,
        Date startDate,
        Date endDate)
    {
        return bookingSrv.createBooking(
            userId,
            vehicleId,
            startDate,
            endDate);
    }

    bool cancelBooking(
        uint bookingId)
    {
        return bookingSrv.cancelBooking(
            bookingId);
    }
};
