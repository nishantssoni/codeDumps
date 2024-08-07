from tkinter import *
from PIL import Image
import PIL.ImageTk as ImageTk


class Cust_win:
    def __init__(self, root):
        
        # initializing root window
        self.root = root
        
        # setting the title
        self.root.title("Customer")
        
        # setting the height and width
        # geometry("heightxwidth+startfromx+startfromy")
        self.root.geometry("1100x500+260+220")

        ################################ Title in image################################################
        lbl_title = Label(self.root, text="Add Customer Details", font=("Times new roman",20),bg="black", fg="gold",bd=1.5,relief=RIDGE)
        lbl_title.place(x=0,y=0,width=1100,height=30)

        ################################# LOGO ################################################

        img_logo = Image.open("img/logo.jpeg")
        img_logo = img_logo.resize((60,30), Image.ANTIALIAS)
        self.root_logo = ImageTk.PhotoImage(img_logo)
        lblimg = Label(self.root, image=self.root_logo, bd=.5, relief=RIDGE)
        lblimg.place(x=0, y=0, width=60, height=30)

        ################################# LOGO ###################################
        # a label frame where we add labels to submit in customer
        lbl_frame = LabelFrame(self.root, bd=1, relief=RIDGE, text="Customer Details", font=("Times new roman",14),pady=4,bg="#cccccc")
        lbl_frame.place(x=0,y=35, height=450, width=400)



if __name__ == "__main__":
    root = Tk()
    obj = Cust_win(root)
    root.mainloop()