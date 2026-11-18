import random
from random import randint
 
movies=[{"id":1,"name":"Oppenheimer","language":"English","price":700,"seats":20,"booked":set()}, {"id":2,"name":"Interstellar","language":"English","price":480,"seats":20,"booked":set()},
   
{"id":3,"name":"Inception","language":"English","price":500,"seats":20,"booked"
:set()},
    {"id":4,"name":"The Wolf Of Wall Street","language":"English","price":670,"seats":20,"booked":set()},
    {"id":5,"name":"The Conjuring","language":"English","price":600,"seats":20,"booked":set()}
]
bookings=[]
def display_movies():
    print("\nAvailable Movies")
    print("-"*60)
    for m in movies:
        print("ID:",m["id"],"| Name:",m["name"],"| Language:",m["language"],"| Price: Rs.",m["price"],"| Seats:",m["seats"])
def calculate_price(price,age):
    if age<12:
        return price*0.5
    elif age>=60:
        return price*0.7
    else:
        return price
def book_ticket():
    try:
        name=input("\nEnter customer name: ")
        movie_id=int(input("Enter Movie ID: "))
        tickets=int(input("Enter number of tickets: "))
        age=int(input("Enter customer age: "))
 
        movie=None
        for m in movies:
            if m["id"]==movie_id:
                movie=m
                break
 
        if movie is None:
            print("Invalid Movie ID!")
            return
 
        if tickets<=0 or age<0:
            print("Invalid number of tickets or age!")
            return
 
        if tickets>movie["seats"]:
            print("Not enough seats available!")
            return
 
        print("Available seats:",end=" ")
        for i in range(1,21):
            if i not in movie["booked"]:
                print(i,end=" ")
        print()
 
        selected=[]
        while len(selected)<tickets:
            try:
                seat=int(input("Enter seat number: "))
 
                if seat<1 or seat>20:
                    print("Invalid seat number!")
                    continue
 
                if seat in movie["booked"] or seat in selected:
                    print("Seat already booked! Select another seat.")
                    continue
 
                selected.append(seat)
 
            except ValueError:
                print("Please enter a valid seat number.")
 
        for seat in selected:
            movie["booked"].add(seat)
 
        movie["seats"]-=tickets
 
        total=0
        ticket_price=calculate_price(movie["price"],age)
        total=ticket_price*tickets
 
        booking_id=randint(1000,9999)
        while any(b["id"]==booking_id for b in bookings):
            booking_id=randint(1000,9999)
 
        booking={
            "id":booking_id,
            "name":name,
            "movie":movie["name"],
            "movie_id":movie_id,
            "tickets":tickets,
            "seats":selected,
            "amount":total,
            "status":"Confirmed"
        }
 
        bookings.append(booking)
 
        print("\nBooking Successful!")
        print("Booking ID:",booking_id)
        print("Total Amount: Rs.",total)
 
    except (ValueError,TypeError):
        print("Invalid input! Please enter valid values.")
    except Exception as e:
        print("Error:",e)
    else:
        print("Booking operation completed.")
    finally:
        print("Thank you for using the booking system.")
 
 
def search_booking():
    try:
        booking_id=int(input("\nEnter Booking ID: "))
 
        for b in bookings:
            if b["id"]==booking_id:
                print("\nBooking Details")
                print("Customer Name:",b["name"])
                print("Movie Name:",b["movie"])
                print("Number of Tickets:",b["tickets"])
                print("Selected Seats:",b["seats"])
                print("Total Amount: Rs.",b["amount"])
                print("Booking Status:",b["status"])
                return
        print("Booking ID not found!")
    except (ValueError,TypeError):
        print("Invalid Booking ID!")
    finally:
        print("Search completed.")
 
def cancel_booking():
    try:
        booking_id=int(input("\nEnter Booking ID: "))
        booking=None
        for b in bookings:
            if b["id"]==booking_id:
                booking=b
                break
        if booking is None:
            print("Booking ID not found!")
            return
        if booking["status"]=="Cancelled":
            print("Booking is already cancelled!")
            return
        for m in movies:
            if m["id"]==booking["movie_id"]:
                for seat in booking["seats"]:
                    m["booked"].remove(seat)
                m["seats"]+=booking["tickets"]
                break
        booking["status"]="Cancelled"
        print("Booking cancelled successfully.")
    except (ValueError,TypeError,KeyError):
        print("Invalid Booking ID or booking information.")
    finally:
        print("Cancellation operation completed.")
 
 
 
while True:
    print("\nMOVIE TICKET BOOKING SYSTEM")
    print("1. Display Movies")
    print("2. Book Tickets")
    print("3. Search Booking")
    print("4. Cancel Booking")
    print("5. Exit")
    try:
        choice=int(input("Enter your choice: "))
    except ValueError:
        print("Please enter a number.")
        continue
    if choice==1:
        display_movies()
    elif choice==2:
        book_ticket()
    elif choice==3:
        search_booking()
    elif choice==4:
        cancel_booking()
    elif choice==5:
        print("Thank you!")
        break
    else:
        print("Invalid choice!")
