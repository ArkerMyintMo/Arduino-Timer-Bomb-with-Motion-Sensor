# Arduino-Timer-Bomb-with-Motion-Sensor
*******This is my project of final part1 in Master Of Computer Technology.*******

Propose system
1. Once we power the bomb, we are asked to set the password and the time we want to give it.
2. Once the password and timer have been set, the bomb will be active. The time is reduced by 1 second.
3. If the enemy tries to diffuse the bomb, press the "*" button to extract the password. If the password is correct, the bomb will be prompted to reset the timer and password, and if the password is incorrect, the current time will be reduced by half. If the enemy repeats the wrong password and the timer == 0, then the bomb is explosive.
4. At the same time, if moving objects come within 3 meters specified by the motion sensor, the current time will be reduced by half. The motion sensor is forced to work in a loop and threshold values are set. When the value in the loop is equal to the threshold value, the motion sensor will reduce the time. If timer == 0, then the bomb is explosive.

Propose system
1.	ကျွန်တော်တို့ bomb ကို power ပေးပြီဆိုတာနဲ့ password နဲ့ မိမိပေးလိုတဲ့ အချိန်ကိုသတ်မှတ်ခိုင်းပါတယ်။
2.	Password ရော timer ရော သတ်မှတ်ပြီးပြီဆိုတာနဲ့ bomb က active စဖြစ်ပါပြီ။ အချိန်ကိုတော့ ၁ စက္ကန့်စီ လျှော့သွားပါတယ်။
3.	ရန်သူအနေနဲ့ bomb ကို diffuse လုပ်ဖို့ကြိုးစားခဲ့မယ်ဆိုရင် တော့ “ * ” button ကိုနှိပ်ပြီး password ကို ဖြည်ရပါမယ်။ password မှန်ခဲ့မယ်ဆိုရင် bomb ကို timer နဲ့ password ပြန်လည်သတ်မှတ်ခိုင်းမှာ ဖြစ်ပြီး အကယ်၍ password မှားခဲ့ရင်တော့ လက်ရှိရောက်နေတဲ့ အချိန် တစ်ဝက်ကို လျှော့ချသွားမှာဖြစ်ပါတယ်။ ရန်သူအနေနဲ့ password ကို ထပ်ခါထပ်ခါ မှားယွင်းခဲ့ ပြီး timer == 0 ဖြစ်သွားရင် တော့ bomb က explosive ဖြစ်ပါပြီ။
4.	တစ်ချိန်တည်းမှာပဲ motion sensor အနေနဲ့ သတ်မှတ်ထားတဲ့ 3 meter အတွင်းမှာ moving objects ဝင်လာပြီဆိုရင် လက်ရှိအချိန်ကို တစ်ဝက်လျှော့ချသွားမှာ ဖြစ်ပါတယ်။ motion sensor ကိုတော့ loop တစ်ခုအတွင်းမှာ အလုပ်လုပ်ခိုင်းထားပြီး threshold values သတ်မှတ်ပေးထားပါတယ်။ loop အတွင်း က value သည် threshold value နဲ့ တူညီတဲ့ အချိန်ကျမှ motion sensor က အချိန်ကို လျှော့ချသွားမှာ ဖြစ်ပါတယ်။ အကယ်၍ timer == 0 ဖြစ်သွားခဲ့ရင်တော့ bomb က explosive ဖြစ်ပါပြီ။
