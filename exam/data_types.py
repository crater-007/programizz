student=[]
student_ids=set()
course=("maths", "pdl", "algo")
def add_student():
    name=input("enter name")
    id=int(input("enter id"))
    if id in student_ids:
        print("id already exists")
        return
    student_ids.add(id)
    marks={}
    selected_cources=input("enter courses").split(",")
    for c in course:
        marks[c]=int(input(f"enter marks for {c}"))
    student.append({"name":name,"id":id,"marks":marks})
def display_students():
    for s in student:
        print(f"name: {s['name']}, id: {s['id']}, marks: {s['marks']}")
def search_student():
    id=int(input("enter id to search"))
    for s in student:
        if s["id"]==id:
            print(f"name: {s['name']}, id: {s['id']}, marks: {s['marks']}")
            return
    print("student not found")
def update_marks():
    id=int(input("enter id to update"))
    for s in student:
        if s["id"]==id:
            for c in course:
                s["marks"][c]=int(input(f"enter marks for {c}"))
            return
    print("student not found")