#include <string.h>
#define DATA 11
#define LATCH 12
#define CLK 8

int mode[] = { 0, 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };
int row1[] = { 32768, 16384, 8192 };
int row2[] = { 512, 256, 128 };

// freez is variable to hold last position
int freez = 0;

// int p1[3][3] = {{0,1,1},{1,0,0},{1,0,1}};
// int p2[3][3] = {{1,0,0},{0,1,1},{0,1,0}};

// int p1[3][3] = {{0,1,1},{1,0,0},{1,0,1}};
// int p2[3][3] = {{1,0,0},{0,1,1},{0,1,0}};

// int p1[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };
// int p2[3][3] = { { 0, 0, 0 }, { 0, 0, 0 }, { 0, 0, 0 } };

int board[3][3] = { { 2, 1, 0 }, { 0, 1, 2 }, { 0, 0, 1 } };

// game var and function

int turn = random(2) + 1;

// int moves_left() {
//   int c = 0;
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       if (p1[i][j] == 1) c++;
//       if (p2[i][j] == 1) c++;
//     }
//   }
//   return c;
// }

// int giveVal(int a, int pl) {
//   int curr = 1;
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       if (curr == a) {
//         if (pl == 1) return p1[i][j];
//         else return p2[i][j];
//       }
//       curr++;
//     }
//   }
// }

// int checkWin() {
//   /* 0 == nothing
//         1 = player first win
//         2 = player second win*/

//   String lst[] = { "789", "456", "123", "741", "852", "963", "753", "951" };

//   for (int i = 0; i < 8; i++) {
//     String a = lst[i];
//     int b = int(a[0]) - 48;
//     int c = int(a[1]) - 48;
//     int d = int(a[2]) - 48;
//     for (int p = 1; p <= 2; p++) {
//       if ((giveVal(b, p) == giveVal(c, p)) && (giveVal(c, p) == giveVal(d, p))) {
//         if ((giveVal(b, p) != 0) && (giveVal(c, p) != 0) && (giveVal(d, p) != 0)) {
//           // cout<<giveVal(b,p)<<giveVal(c,p)<<giveVal(d,p)<<endl;
//           // cout<<b<<c<<d<<endl;
//           // cout<<p<<endl;
//           clear();
//           freeFun(b, p);
//           freeFun(c, p);
//           freeFun(d, p);
//           freez = 1;

//           return p;
//         }
//       }
//     }
//   }

//   return 0;
// }



// int checkTie() {
//   if (moves_left() == 9) {
//     if (checkWin() == 0) return 1;
//   }
//   return 0;
// }

// int placeMove(int a) {
//   /*1 = successfully
//         0 = already fill or no chance
//         */
//   if (moves_left() == 9) return 0;
//   int curr = 1;
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       if (curr == a) {
//         if (turn == 1) {
//           if (p1[i][j] == 0 && p2[i][j] == 0) {
//             p1[i][j] = 1;
//             return 1;
//           }
//         } else if (turn == 2) {
//           if (p2[i][j] == 0 && p1[i][j] == 0) {
//             p2[i][j] = 1;
//             return 1;
//           }
//         }
//       }
//       curr++;
//     }
//   }
//   return 0;
// }

// int win_or_tie() {
//   //1 - win
//   //0 - nothing
//   //2 for tie
//   if (checkWin()) return 1;
//   if (checkTie()) return 2;
//   return 0;
// }

// void clear() {
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       p2[i][j] = 0;
//       p1[i][j] = 0;
//     }
//   }
// }
// void freeFun(int a,int p) {
//   int curr = 1;
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       if (curr == a) {
//         if (p == 1) p1[i][j] = 1;
//         else p2[i][j] = 1;
//       }
//       curr++;
//     }
//   }
// }


// // cirtuit function and var

// void off() {
//   update(4096 + 2048 + 1024);
// }

int g_grd(int index) {
  int sum = 0;
  int grd[] = { 4096, 2048, 1024 };
  for (int i = 0; i < 3; i++) {
    if (index == i) continue;
    sum += grd[i];
  }
  return sum;
}

// void fun() {
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       if (p1[i][j] == 1) {
//         update((g_grd(j) + row1[i]));
//       }
//     }
//   }
// }

void ledGlow() {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == 1) {
        update((g_grd(j) + row1[i]));
      }
    }
  }

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      if (board[i][j] == 2) {
        update((g_grd(j) + row2[i]));
      }
    }
  }
}


// void fun1() {
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {
//       if (p2[i][j] == 1) {
//         update((g_grd(j) + row2[i]));
//       }
//     }
//   }
// }


void update(int a) {
  digitalWrite(LATCH, LOW);
  shiftOut(DATA, CLK, LSBFIRST, a);
  shiftOut(DATA, CLK, LSBFIRST, a >> 8);
  digitalWrite(LATCH, HIGH);
}

// void drawBoard() {
//   fun();
//   delay(1);
//   fun1();
//   delay(1);
// }
// void draw() {
//   Serial.println("Welcome to tictactoe");
//   Serial.print("PLAYER ");
//   Serial.println(turn);
//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {

//       Serial.print(p1[i][j]);
//     }
//     Serial.println();
//   }
//   Serial.println("-----------------------------");

//   for (int i = 0; i < 3; i++) {
//     for (int j = 0; j < 3; j++) {

//       Serial.print(p2[i][j]);
//     }
//     Serial.println();
//   }
// }




void setup() {
  pinMode(DATA, OUTPUT);
  pinMode(LATCH, OUTPUT);
  pinMode(CLK, OUTPUT);
  Serial.begin(9600);
  // off();
  // draw();
}



void loop() {

  ledGlow();


  // drawBoard();
  // if (freez) {
  //   drawBoard();
  // } else {
  //   if (win_or_tie() == 1) {
  //     Serial.println("Win");
  //     Serial.println(turn);
  //     clear();
  //   } else if (win_or_tie() == 2) {
  //     Serial.println("Tie");
  //     clear();
  //   }

  //   while (Serial.available() > 0) {
  //     int data = (int)Serial.parseInt();
  //     if (data > 0) {
  //       if (win_or_tie() == 0) {

  //         if (placeMove(data)) {
  //           turn = (turn == 1) ? 2 : 1;
  //         }


  //         draw();
  //       }
  //     }
  //   }
  // }
}
