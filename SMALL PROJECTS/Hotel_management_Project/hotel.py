from tkinter import *
from PIL import Image
import PIL.ImageTk as ImageTk
from customer import Cust_win

class HotelManagementSystem:
    def __init__(self, root):
        
        # initializing root window
        self.root = root
        
        # setting the title
        self.root.title("Hotel Management System")
        
        # setting the height and width
        # geometry("heightxwidth+startfromx+startfromy")
        self.root.geometry("1550x800+0+0")


####################### IMAGE BANNER #####################################
        # Images
        img_banner = Image.open("img/hotel_banner.jpg")
        
        # resizing image and ANTIAlIAS make high quality image to low quality
        img_banner = img_banner.resize((1550,100), Image.ANTIALIAS)
        
        # setting our image to root and adding our image to photoimage
        self.root_bg = ImageTk.PhotoImage(img_banner)
        
        # for showing image we have to make lable
        # seeting our image to lable class and giving them boder
        lblimg = Label(self.root, image=self.root_bg, bd=.5, relief=RIDGE)

        # placing our image in lable in window ,place(startx,starty, width, height)
        lblimg.place(x=200, y=0, width=1550, height=100)

################################# LOGO ################################################

        img_logo = Image.open("img/logo.jpeg")
        
        # resizing image and ANTIAlIAS make high quality image to low quality
        img_logo = img_logo.resize((200,100), Image.ANTIALIAS)
        
        # setting our image to root and adding our image to photoimage
        self.root_logo = ImageTk.PhotoImage(img_logo)
        
        # for showing image we have to make lable
        # seeting our image to lable class and giving them boder
        lblimg = Label(self.root, image=self.root_logo, bd=.5, relief=RIDGE)

        # placing our image in lable in window ,place(startx,starty, width, height)
        lblimg.place(x=0, y=0, width=200, height=100)


################################ Title in image################################################
        lbl_title = Label(self.root, text="Hotel Management System", font=("Times new roman",40),bg="black", fg="gold",bd=1.5,relief=RIDGE)
        lbl_title.place(x=0,y=100,width=1550,height=50)



################################ Main Frame ###########################################

        main_frame = Frame(self.root, bd=.5, relief=RIDGE)
        main_frame.place(x=0, y=150, width=1550, height=650)


################################ Menu ###########################################
        # we want our menu inside the main_from not the self.root
        lbl_menu = Label(main_frame, text="Menu", font=("Times new roman",20,"bold"),bg="black", fg="gold",bd=1.5,relief=RIDGE)
        # if we does't give height then the height is set according to the font
        lbl_menu.place(x=0,y=0,width=230,height=30)


################################ Btn Frame ###########################################

        btn_frame = Frame(main_frame, bd=.5, relief=RIDGE)
        btn_frame.place(x=0, y=30, width=230, height=190)


################################ button  ###########################################
        cust_btn = Button(btn_frame, command=self.cust_details, text="Customer", cursor="hand2", width=20, font=("Times new roman",15),bg="#2874A6", fg="white",bd=0,relief=RIDGE)
        cust_btn.grid(row=0, column=0,pady=3)

        room_btn = Button(btn_frame, text="Room", cursor="hand2", width=20, font=("Times new roman",15),bg="#2874A6", fg="white",bd=0,relief=RIDGE)
        room_btn.grid(row=1, column=0,pady=3)

        detail_btn = Button(btn_frame, text="Detail", cursor="hand2", width=20, font=("Times new roman",15),bg="#2874A6", fg="white",bd=0,relief=RIDGE)
        detail_btn.grid(row=2, column=0,pady=3)
        
        report_btn = Button(btn_frame, text="Report", cursor="hand2", width=20, font=("Times new roman",15),bg="#2874A6", fg="white",bd=0,relief=RIDGE)
        report_btn.grid(row=3, column=0,pady=3)

        logout_btn = Button(btn_frame, text="Logout", cursor="hand2", width=20, font=("Times new roman",15),bg="#FF605C", fg="white",bd=0,relief=RIDGE)
        logout_btn.grid(row=4, column=0,pady=3)


# =============================== background image =============================

        img_back = Image.open("img/hotelbg.jpg")
        img_back = img_back.resize((1320,590), Image.ANTIALIAS)
        
        self.img_back = ImageTk.PhotoImage(img_back)
        
        lbl_back = Label(main_frame, image=self.img_back, bd=.5, relief=RIDGE)

        lbl_back.place(x=230, y=0, width=1320, height=590)

# =============================== Customer windows =============================

    def cust_details(self):
        # setting new window in top level of root
        self.new_window = Toplevel(self.root)
        # adding our new window in cust_window
        self.app = Cust_win(self.new_window)



if __name__ == "__main__":
    # giving root a toolkin TK
    root = Tk()
    # passing root to class
    obj = HotelManagementSystem(root)
    # closing mainloop
    root.mainloop()