def generate_bill(*items, **details):
    subtotal=0
    print("Bill Details:")
    for i in items:
        name, qty, price=i
        total=qty*price
        subtotal+=total
        print(f"{name}: {qty} x {price} = {total}")
    print(f"Subtotal: {subtotal}")
    tax=details["tax"]
    discount=details["discount"]
    discount_amt=subtotal*discount/100
    
    tax_amt=subtotal-discount_amt * tax/100
    total=subtotal+tax-discount_amt
    print(f"Tax: {tax_amt}")
    print(f"Total: {total}")
n=int(input("enter number of items"))
items=[]
for i in range(n):
    name=input("enter item name")
    qty=int(input("enter quantity"))
    price=float(input("enter price"))
    items.append((name, qty, price))
tax=float(input("enter tax percentage"))
discount=float(input("enter discount percentage"))
generate_bill(*items, tax=tax, discount=discount)