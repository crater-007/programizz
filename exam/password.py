password=input("Enter your password: ")
upper=False
lower=False
digit=False
special=False
if len(password)<8:
    print("Password must be at least 8 characters long")
else:
    for char in password:
        if char.isupper():
            upper=True
        elif char.islower():
            lower=True
        elif char.isdigit():
            digit=True
        elif char in "!@#$%^&*()-+":
            special=True
    if not upper:
        print("Password must contain at least one uppercase letter")
    if not lower:
        print("Password must contain at least one lowercase letter")
    if not digit:
        print("Password must contain at least one digit")
    if not special:
        print("Password must contain at least one special character")