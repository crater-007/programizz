with open("students.txt","w") as file:
    file.write("name,id,maths,pdl,algo\n")
    file.write("Alice,1,90,85,80\n")
    file.write("Bob,2,75,80,70\n")
with open("students.txt","r") as file:
    data=file.read()
    print(data)
with open("students.txt","a") as file:
    file.write("Charlie,3,85,90,95\n")
with open("students.txt","r") as file:
    data=file.read()
    print(data)