// #include "C:/Wichtig/System/Static/Library/WindowEngine1.0.h"
// #include "C:/Wichtig/System/Static/Library/Victor2.h"
// #include "C:/Wichtig/System/Static/Library/Vdctor2.h"
// #include "C:/Wichtig/System/Static/Library/Complex.h"
// #include "C:/Wichtig/System/Static/Library/TransformedView.h"
// #include "C:\Wichtig\System\Static\Container\Vector.h"

#include "/home/codeleaded/System/Static/Library/WindowEngine1.0.h"
#include "/home/codeleaded/System/Static/Library/TransformedView.h"
#include "/home/codeleaded/System/Static/Container/Vector.h"


TransformedView tv;

Vec2 outer_p;
float outer_r;

Vec2 ball_p;
float ball_r;
Vec2 ball_v;
Vec2 ball_a;

void Setup(AlxWindow* w){
	tv = TransformedView_New((Vec2){ GetWidth(),GetHeight() });
	TransformedView_Offset(&tv,(Vec2){ 0.0f,0.0f });

	outer_p = (Vec2){ 0.0f,0.0f };
	outer_r = 1.0f;

	ball_p = (Vec2){ 0.0f,0.0f };
	ball_r = 0.05f;
	ball_v = (Vec2){ 4.0f,0.0f };
	ball_a = (Vec2){ 0.0f,10.0f };

	ResizeAlxFont(16,16);
}
void Update(AlxWindow* w){
	tv.ZoomSpeed = (float)w->ElapsedTime;
	TransformedView_HandlePanZoom(&tv,window.Strokes,(Vec2){ GetMouse().x,GetMouse().y });

	ball_v = Vec2_Add(ball_v,Vec2_Mulf(ball_a,w->ElapsedTime));
	ball_p = Vec2_Add(ball_p,Vec2_Mulf(ball_v,w->ElapsedTime));

	const Vec2 d = Vec2_Sub(ball_p,outer_p);
	const float l = outer_r - ball_r;
	const float dist = Vec2_Mag(d);
	if(dist>l){
		ball_v = Vec2_Reflect(Vec2_Perp(d),ball_v);
		if(ball_r<outer_r) 	ball_r *= 1.05f;
		else 				ball_r = outer_r;
		ball_p = Vec2_Add(outer_p,Vec2_Mulf(Vec2_Norm(d),outer_r - ball_r));
	}

	Clear(BLACK);

	
	Vec2 bg_p = TransformedView_WorldScreenPos(&tv,outer_p);
	float bg_r = TransformedView_WorldScreenLX(&tv,outer_r);

	int op_r = (int)(ball_r * 10000.0f) & 255;
	int op_g = (int)(ball_r * 1000.0f) & 255;
	int op_b = (int)(ball_r * 100.0f) & 255;
	Pixel op_c = Pixel_RGBA(op_r,op_g,op_b,255);
	RenderCircle(bg_p,bg_r,op_c);


	Vec2 p = TransformedView_WorldScreenPos(&tv,ball_p);
	float r = TransformedView_WorldScreenLX(&tv,ball_r);
	
	int p_r = (int)(ball_r * 500000.0f) & 255;
	int p_g = (int)(ball_r * 50000.0f) & 255;
	int p_b = (int)(ball_r * 5000.0f) & 255;
	Pixel p_c = Pixel_RGBA(p_r,p_g,p_b,255);
	RenderCircle(p,r,p_c);


	String str = String_Format("| Offset: %f,%f - Zoom: %f,%f |",tv.Offset.x,tv.Offset.y,tv.Scale.x,tv.Scale.y);
	RenderCStrSize(str.Memory,str.size,0,0,WHITE);
	String_Free(&str);
}
void Delete(AlxWindow* w){
    
}

int main(){
    if(Create("TT Bouncing Ball",1200,1200,1,1,Setup,Update,Delete))
        Start();
    return 0;
}