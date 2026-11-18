from abc import ABC, abstractmethod
class item:
    def __init__(self, name, price, quantity):
        self.name= name
        self.price= price
        self.quantity= quantity
    def get_total(self):
        return self.price*self.quantity

class DiscountStrategy(ABC):
    @abstractmethod
    def calculate_discount(self, amount):
        pass
class NoDiscount(DiscountStrategy):
    def calculate_discount(self, amount):
        return 0
class PercentageDiscount(DiscountStrategy):
    def __init__(self, percentage):
        self.percentage = percentage
    def calculate_discount(self, amount):
        return amount * (self.percentage / 100)
class FlatDiscount(DiscountStrategy):
    def __init__(self, amount):
        self.amount=amount
    def calculate_discount(self, amount):
        return min(self.amount, amount)

class ShoppingCart:
    def __init__(self, discount_strategy):
        self.items=[]
        self.discount_strategy=discount_strategy
    def add_item(self, item):
        self.items.append(item)
    def calculate_subtotal(self):
        return sum(item.get_total() for item in self.items)
def checkout(cart):
    subtotal=cart.calculate_subtotal()
    discount=cart.discount_strategy.calculate_discount(subtotal)
    total=subtotal-discount
    return total

item1 = item("Laptop", 60000, 1)
item2 = item("Mouse", 1200, 2)
item3 = item("Keyboard", 2500, 1)


discount = PercentageDiscount(10)


cart = ShoppingCart(discount)


cart.add_item(item1)
cart.add_item(item2)
cart.add_item(item3)


total = checkout(cart)
print(f"Total: {total}")
