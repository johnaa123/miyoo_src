

--苍龙逐日1.2修改部分，替代原来的jymodify.lua即可






---本模块存放对JYMain.lua 的修改和扩充。

--尽量把新增加模块放在这里，少修改原始JYMain.Lua文件。
--这里一般包括以下几个部分
--1. SetModify函数。   该函数在游戏开始时调用，可以在此修改原有的数据，以及重定义原有的函数，以实现对原有函数的修改、
--                    这样就可以基本不动原始的函数
--2. 原有函数的重载函数。 SetModify中重载的函数放在此处。尽量不修改JYMain.lua文件，对它的修改采用重定义函数的形式。
--3. 新的物品使用函数。
--4. 新的场景事件函数。





--对jymain的修改，以及增加新的物品函数和场景事件函数。
--注意这里可以定义全程变量。
function SetModify()

   --这是一个定义函数的例子。这里重新修改主菜单中的系统菜单，增加在游戏运行中控制音效的功能。
   --原来只能在jyconst.lua中通过参数在运行前控制，不能做到实时控制。
   Menu_System_old=Menu_System;         --备份原始函数，如果新的函数需要，还可以调用原始函数。
   Menu_System=Menu_System_new;

   ExecDoctor_old=ExecDoctor;         --新的医疗函数
   ExecDoctor=ExecDoctor_new;

   War_PersonLostLife_old=War_PersonLostLife;         --新的中毒减少生命函数
   War_PersonLostLife=War_PersonLostLife_new;

   instruct_11_old=instruct_11;         --新的指令
   instruct_11=instruct_11_new;

   instruct_45_old=instruct_45;
   instruct_45=instruct_45_new;

   instruct_46_old=instruct_46;
   instruct_46=instruct_46_new;

   instruct_47_old=instruct_47;
   instruct_47=instruct_47_new;

   instruct_48_old=instruct_48;
   instruct_48=instruct_48_new;

   --在此定义特殊物品。没有定义的均调用缺省物品函数
    JY.ThingUseFunction[182]=Show_Position;     --罗盘函数


--修改CC的常量

   --各种贴图文件名。
   CC.SMAPPicFile={CONFIG.DataPath .. "sdx",CONFIG.DataPath .. "smp"};
   CC.WMAPPicFile={CONFIG.DataPath .. "wdx",CONFIG.DataPath .. "wmp"};
   CC.FightPicFile={CONFIG.DataPath .. "fdx%03d",CONFIG.DataPath .. "fmp%03d"};  --此处为字符串格式，类似于C中printf的格式。

       --离队人员列表: {人员id，离队调用函数}      ----如果有新的离队人员加入，直接在这里增加
    CC.PersonExit={{1,100},{2,102},{4,104},{9,106},{16,108},
                   {17,110},{25,112},{28,114},{29,116},{30,118},
                   {35,120},{36,122},{37,124},{38,126},{44,128},
                   {45,130},{47,132},{48,134},{49,136},{51,138},
                   {52,140},{53,142},{54,144},{55,146},{56,148},
                   {58,150},{59,152},{63,154},{66,156},{72,158},
                   {73,160},{74,162},{75,164},{76,166},{77,168},
                   {78,170},{79,172},{80,174},{81,176},{82,178},
                   {83,180},{84,182},{85,184},{86,186},{87,188},
                   {88,190},{89,192},{90,194},{91,196},{92,198},

				   };
				   
       --离队人员列表: {人员id，离队调用函数}      ----如果有新的离队人员加入，直接在这里增加
    CC.PersonExit={{1,100},{2,102},{4,104},{9,106},{16,108},
                   {17,110},{25,112},{28,114},{29,116},{30,118},
                   {35,120},{36,122},{37,124},{38,126},{44,128},
                   {45,130},{47,132},{48,134},{49,136},{51,138},
                   {52,140},{53,142},{54,144},{55,146},{56,148},
                   {58,150},{59,152},{63,154},{66,156},{72,158},
                   {73,160},{74,162},{75,164},{76,166},{77,168},
                   {78,170},{79,172},{80,174},{81,176},{82,178},
                   {83,180},{84,182},{85,184},{86,186},{87,188},
                   {88,190},{89,192},{90,194},{91,196},{92,198},
                   {589,200},{590,202},{591,204},{592,206},{593,208},
				   {594,210},{595,212},{596,214},{97,216},{597,218},
				   {50,220},{598,222},{599,224},{600,226},{601,228},
				   {103,230},{602,232},{603,234},{604,236},{605,238},
				   {606,240},{607,242},{608,244},{609,246},{610,248},
				   {93,250},{611,252},
				   };
				   


    CC.ExtraOffense={{52,75,100},             --武功武器配合增加攻击力， 依次为：武器物品id，武功id，攻击力增加
                   {45,67,100},
                   {37,41,100},
                   {49,80,200},
                   {44,63,150},
                   {40,40,150},
                   {36,45,100},}
				   
  CC.Effect={[0]=10,14,17,9,13,                    --eft.idx/grp
                 17,17,17,18,19,
                 19,15,13,10,10,
                 15,21,16,9,11,
                 8,9,8,8,7,
                 8,8,9,12,19,
                 11,14,12,17,8,
                 11,9,13,10,19,
                 14,17,19,14,21,
                 16,13,18,14,17,
                 17,16,7,14,14,
 				 14,8,8,8,8,
				 8,15,14,7,7,
				 10,9,9,9,9,
				 9,19,19,19,19,
				 18,18,18,12,12,
				 12,12,14,14,14,
				 16,16,10,10,10,
				 8,15,17,17,14,
				 14,9,9,18,13,
				 8,17,9,16,21,
				 14,13,12,14,16,
				 20,19,14,19,20,
				 12,12,12,12,12,
				 12,12,12,12,12,
				 9,7,15,10,11,
				 11,13,6,10,10,
				 13,10,10,19,16,
				 14,14,15,12,7,
				 9,13,11,12,13,
				 21,21,12,12,6,
				 12,20,6,12,21,
				 15,14,13,13,7,
				 15,14,14,15,12,
				 15,16,14,20,13,
				 15,13,13,11,16	,			 
				 16,15,16,8,16,
				 16,19,10,14,16,
				 18,12,12,7,8,
				 13,17,15,17,26,};				   

   CC.PersonAttribMax={};             --人物属性最大值
   CC.PersonAttribMax["经验"]=60000;
   CC.PersonAttribMax["物品修炼点数"]=60000;
   CC.PersonAttribMax["修炼点数"]=60000;
   CC.PersonAttribMax["生命最大值"]=9999;
   CC.PersonAttribMax["受伤程度"]=100;
   CC.PersonAttribMax["中毒程度"]=100;
   CC.PersonAttribMax["内力最大值"]=9999;
   CC.PersonAttribMax["体力"]=100;
   CC.PersonAttribMax["攻击力"]=999;
   CC.PersonAttribMax["防御力"]=999;
   CC.PersonAttribMax["轻功"]=999;
   CC.PersonAttribMax["医疗能力"]=999;
   CC.PersonAttribMax["用毒能力"]=999;
   CC.PersonAttribMax["解毒能力"]=999;
   CC.PersonAttribMax["抗毒能力"]=999;
   CC.PersonAttribMax["拳掌功夫"]=999;
   CC.PersonAttribMax["御剑能力"]=999;
   CC.PersonAttribMax["耍刀技巧"]=999;
   CC.PersonAttribMax["特殊兵器"]=999;
   CC.PersonAttribMax["暗器技巧"]=999;
   CC.PersonAttribMax["武学常识"]=999;
   CC.PersonAttribMax["品德"]=100;
   CC.PersonAttribMax["资质"]=100;
   CC.PersonAttribMax["攻击带毒"]=999;


    CC.NewPersonName="徐小侠";                --新游戏的数据
    CC.NewGameSceneID=70;                      --场景ID
    CC.NewGameSceneX=16;                       --场景坐标
    CC.NewGameSceneY=31;
    CC.NewGameEvent=691;                       --新游戏场景执行事件。如果没有，则执行新游戏坐标路过事件。
    CC.NewPersonPic=2515;                      --开始主角pic


   if CONFIG.MP3==1 then
      CC.MIDIFile=CONFIG.SoundPath .. "game%02d.mp3";
   else
      CC.MIDIFile=CONFIG.SoundPath .. "game%02d.wav";
   end
   
    CC.LoadThingPic=1              --读取物品贴图方式 0 从mmap/smap/wmap中读取  1 读取独立的thing.idx/grp
	CC.StartThingPic=4131          --物品贴图在mmap/smap/wmap中的起始编号。CC.LoadThingPic=0有效


end


--新的系统子菜单，增加控制音乐和音效
function Menu_System_new()
	local menu={
	             {"读取进度",Menu_ReadRecord,1},
                 {"保存进度",Menu_SaveRecord,1},
				 {"关闭音乐",Menu_SetMusic,1},
				 {"关闭音效",Menu_SetSound,1},
				 {"全屏切换",Menu_FullScreen,1},
                 {"离开游戏",Menu_Exit,1},   };

    if JY.EnableMusic==0 then
	    menu[3][1]="打开音乐";
	end

	if JY.EnableSound==0 then
	    menu[4][1]="打开音效";
    end


    local r=ShowMenu(menu,6,0,CC.MainSubMenuX,CC.MainSubMenuY,0,0,1,1,CC.DefaultFont,C_ORANGE, C_WHITE);
    if r == 0 then
        return 0;
    elseif r<0 then   --要退出全部菜单，
        return 1;
 	end
end

function Menu_FullScreen()
    lib.FullScreen();
	lib.Debug("finish fullscreen");
end

function Menu_SetMusic()
    if JY.EnableMusic==0 then
	    JY.EnableMusic=1;
		PlayMIDI(JY.CurrentMIDI);
	else
	    JY.EnableMusic=0;
		lib.PlayMIDI("");
	end
	return 1;
end

function Menu_SetSound()
    if JY.EnableSound==0 then
	    JY.EnableSound=1;
	else
	    JY.EnableSound=0;
	end
	return 1;
end


----------------------------------------------------------------
---------------------------物品使用函数--------------------------


--罗盘函数，显示主地图主角位置
function Show_Position()
    if JY.Status ~=GAME_MMAP then
        return 0;
    end
    DrawStrBoxWaitKey(string.format("当前位置(%d,%d)",JY.Base["人X"],JY.Base["人Y"]),C_ORANGE,CC.DefaultFont);
	return 1;
end


--执行医疗
--id1 医疗id2, 返回id2生命增加点数
function ExecDoctor_new(id1,id2)      --执行医疗
	if JY.Person[id1]["体力"]<50 then
        return 0;
	end

	local add=JY.Person[id1]["医疗能力"];
    local value=JY.Person[id2]["受伤程度"];
    if value > add+20 then
        return 0;
	end

    if value <25 then    --根据受伤程度计算实际医疗能力
        add=add*4/2;         --此处修改
      --add=add*4/5;
	elseif value <50 then
        add=add*3/4;
	elseif value <75 then
        add=add*2/3;
	else
        add=add/2;
	end
 	add=math.modf(add)+Rnd(5);

    AddPersonAttrib(id2,"受伤程度",-add);
    return AddPersonAttrib(id2,"生命",add)
end



function War_PersonLostLife_new()             --计算战斗后每回合由于中毒或受伤而掉血
    for i=0,WAR.PersonNum-1 do
        local pid=WAR.Person[i]["人物编号"]
        if WAR.Person[i]["死亡"]==false then
            if JY.Person[pid]["受伤程度"]>0 then
                local dec=math.modf(JY.Person[pid]["受伤程度"]/20);
                AddPersonAttrib(pid,"生命",-dec);
            end
            if JY.Person[pid]["中毒程度"]>0 then
                --local dec=math.modf(JY.Person[pid]["中毒程度"]/10);
                local dec=math.modf(JY.Person[pid]["中毒程度"]);          --此处修改
                AddPersonAttrib(pid,"生命",-dec);
            end
            if JY.Person[pid]["生命"]<=0 then
                JY.Person[pid]["生命"]=1;
            end
        end
    end
end


function instruct_11_new()              --是否住宿
    Cls();
    return DrawStrBoxYesNo(-1,-1,"是否(Y/N)?",C_ORANGE,CC.DefaultFont);
end


function instruct_45_new(id,value)        --增加轻功
    local add,str=AddPersonAttrib(id,"轻功",value);
end


function instruct_46_new(id,value)            --增加内力
    local add,str=AddPersonAttrib(id,"内力最大值",value);
    AddPersonAttrib(id,"内力",0);
end


function instruct_47_new(id,value)
    local add,str=AddPersonAttrib(id,"攻击力",value);           --增加攻击力
end


function instruct_48_new(id,value)         --增加生命
    local add,str=AddPersonAttrib(id,"生命最大值",value);
    AddPersonAttrib(id,"生命",0);
end
