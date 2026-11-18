class mediaitem:
    def __init__(self, title, item_id, year):
        self.__title = title
        self.__item_id = item_id
        self._year=year
    def get_title(self):
        return self.__title
    def get_item_id(self):
        return self.__item_id
    def get_year(self):
        return self._year
    def display_item(self):
        print("Title:", self.__title)
        print("Item ID:", self.__item_id)
        print("Year:", self._year)
class book(mediaitem):
    def __init__(self, title, item_id,year, author, pages):
        super().__init__(title,item_id, year)
        self.author = author
        self.pages = pages
    def display_item(self):
        super().display_item()
        print("Author:", self.author)
        print("Pages:", self.pages)
class magazine(mediaitem):
    def __init__(self, title, item_id, year, issue):
        super().__init__(title,item_id, year)
        self.issue = issue
    def display_item(self):
        super().display_item()
        print("Issue:", self.issue)
m1=mediaitem("Generic Media", 1001, 2020)
b1=book("The Great Gatsby", 1002, 1925, "F. Scott Fitzgerald", 218)
mag1=magazine("National Geographic", 1003, 2021, "March 2021")
m1.display_item()
b1.display_item()
mag1.display_item()
#polymorphism
def display_media(items):
    for item in items:
        item.display_item()

display_media([m1, b1, mag1])