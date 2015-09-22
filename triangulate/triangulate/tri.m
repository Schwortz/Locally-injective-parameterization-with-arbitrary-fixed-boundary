function varargout = tri(varargin)
% TRI MATLAB code for tri.fig
%      TRI, by itself, creates a new TRI or raises the existing
%      singleton*.
%
%      H = TRI returns the handle to a new TRI or the handle to
%      the existing singleton*.
%
%      TRI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in TRI.M with the given input arguments.
%
%      TRI('Property','Value',...) creates a new TRI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before tri_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to tri_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help tri

% Last Modified by GUIDE v2.5 15-Jul-2015 17:50:38

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @tri_OpeningFcn, ...
                   'gui_OutputFcn',  @tri_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before tri is made visible.
function tri_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to tri (see VARARGIN)

% Choose default command line output for tri
handles.output = hObject;
handles.flag=0;
handles.flag2=0;
handles.tri_flag=0;
handles.simp_flag=0;
handles.xmin=0;
handles.xmax=1;
handles.ymin=0;
handles.ymax=1;
handles.tri_plot=0;
handles.plots = [];
handles.simPlots = [];
% Update handles structure
guidata(hObject, handles);

% UIWAIT makes tri wait for user response (see UIRESUME)
% uiwait(handles.figure1);


% --- Outputs from this function are returned to the command line.
function varargout = tri_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --- Executes on button press in newBut.
function newBut_Callback(hObject, eventdata, handles)
% hObject    handle to newBut (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if handles.flag==1
    set(handles.poly,'Visible','off')
    for i=1:numel(handles.plots)
        set(handles.plots(i),'Visible','off')
    end
    for i=1:numel(handles.ver_in_plots)
        set(handles.ver_in_plots(i),'Visible','off')
    end
end
if handles.simp_flag == 1
    for i=1:numel(handles.simPlots)
        set(handles.simPlots,'Visible','off')
    end
    handles.simp_flag=0;
end


hold off
handles.poly = impoly();
hold on

handles.flag=1;
temp=handles.poly.getPosition;%flipud( handles.poly.getPosition );

tempx=temp(:,1);
tempy=temp(:,2);
handles.hcmenu = zeros(1,numel(tempx)); %uicontextmenu;
handles.ver_in=zeros(1,numel(tempx));
handles.ver_in_plots=zeros(1,numel(tempx));

for i=1:numel(tempx)
    handles.ver_in_plots(i)=plot(tempx(i),tempy(i));
    handles.hcmenu(i)=uicontextmenu;
    item1 = uimenu(handles.hcmenu(i), 'Label', sprintf('%i',handles.ver_in(i)));
    item2 = uimenu(handles.hcmenu(i), 'Label', '+','callback',{@plus_in,i});
    item3 = uimenu(handles.hcmenu(i), 'Label', '-','callback',{@minus_in,i});
    set(handles.ver_in_plots(i), 'uicontextmenu', handles.hcmenu(i));
end

fid = fopen('out.txt','w');
fprintf(fid, '%i\r\n' , length(temp));
for i=1:length(temp)
    fprintf(fid ,'%f %f %i\r\n' , temp(i,:),handles.ver_in(i));
end
fclose(fid);
hold on
handles.plots = [];
if handles.tri_flag==1
    system('C:\School\Project\triangulate\Release\triangulate.exe');

    fid = fopen('outC.txt','r');
    readval = fscanf(fid,'%f');
    fclose(fid);
    x=zeros(1,4);
    y=zeros(1,4);

    for i=1:numel(readval)
     if ( rem(i,2) == 1 )
         if (rem(i,6)==1)
                x(1) = readval(i);
         else
                if (rem(i,6)==3)
                 x(2)=readval(i);
                else
                 if (rem(i,6)==5)
                    x(3)=readval(i);
                 end
                end
         end

     else
        if (rem(i,6)==2)
            y(1) = readval(i);
        else
            if (rem(i,6)==4)
                y(2)=readval(i);
            else
                if (rem(i,6)==0)
                    y(3)=readval(i);
                end
            end
        end
    end
        if ( rem(i,6) == 0 )
         x(4) = x (1);
         y(4) = y (1);
         handles.plots=[ handles.plots plot(x,y,'r') ];
        end
    end
end
guidata(hObject,handles);
id = addNewPositionCallback(handles.poly,@(p) pos_callback(p,hObject, eventdata, handles));

% --- Executes on button press in triBut.
function triBut_Callback(hObject, eventdata, handles)
% hObject    handle to triBut (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
if handles.flag2==1
    set(handles.tri_plot,'Visible','off');
end
handles.tri_flag=1;
system('C:\School\Project\triangulate\Release\triangulate.exe');

fid = fopen('outC.txt','r');
readval = fscanf(fid,'%f');
fclose(fid);
x=zeros(1,4);
y=zeros(1,4);

for i=1:numel(readval)
    if ( rem(i,2) == 1 )
        if (rem(i,6)==1)
            x(1) = readval(i);
        else
            if (rem(i,6)==3)
                x(2)=readval(i);
            else
                if (rem(i,6)==5)
                    x(3)=readval(i);
                end
            end
        end

    else
        if (rem(i,6)==2)
            y(1) = readval(i);
        else
            if (rem(i,6)==4)
                y(2)=readval(i);
            else
                if (rem(i,6)==0)
                    y(3)=readval(i);
                end
            end
        end
    end
    if ( rem(i,6) == 0 )
        x(4) = x (1);
        y(4) = y (1);
        handles.plots=[ handles.plots plot(x,y,'r') ];
    end
end

guidata(hObject, handles);


% --- Executes on button press in hideBut.
function hideBut_Callback(hObject, eventdata, handles)
% hObject    handle to hideBut (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
for i=1:numel(handles.plots)
    set(handles.plots(i),'Visible','off')
end
handles.tri_flag=0;
guidata(hObject, handles);

function pos_callback(pos,hObject, eventdata, handles)
myhandles = guidata(gcbo);
temp = pos;
for i=1:numel(myhandles.plots)
    set(myhandles.plots(i),'Visible','off')
end


temp=myhandles.poly.getPosition;%flipud( handles.poly.getPosition );
tempx=temp(:,1);
tempy=temp(:,2);

for i=1:numel(tempx)
    myhandles.ver_in_plots(i)=plot(tempx(i),tempy(i));
    myhandles.hcmenu(i)=uicontextmenu;
    item1 = uimenu(myhandles.hcmenu(i), 'Label', sprintf('%i',myhandles.ver_in(i)));
    item2 = uimenu(myhandles.hcmenu(i), 'Label', '+','callback',{@plus_in,i});
    item3 = uimenu(myhandles.hcmenu(i), 'Label', '-','callback',{@minus_in,i});
    set(myhandles.ver_in_plots(i), 'uicontextmenu', myhandles.hcmenu(i));

end

fid = fopen('out.txt','w');
fprintf(fid, '%i\r\n' , length(temp));
for i=1:length(temp)
    fprintf(fid ,'%f %f %i\r\n' , temp(i,:),myhandles.ver_in(i));
end
fclose(fid);

hold on
myhandles.plots = [];

if myhandles.tri_flag==1
system('C:\School\Project\triangulate\Release\triangulate.exe');

fid = fopen('outC.txt','r');
readval = fscanf(fid,'%f');
fclose(fid);
x=zeros(1,4);
y=zeros(1,4);

for i=1:numel(readval)
    if ( rem(i,2) == 1 )
        if (rem(i,6)==1)
            x(1) = readval(i);
        else
            if (rem(i,6)==3)
                x(2)=readval(i);
            else
                if (rem(i,6)==5)
                    x(3)=readval(i);
                end
            end
        end

    else
        if (rem(i,6)==2)
            y(1) = readval(i);
        else
            if (rem(i,6)==4)
                y(2)=readval(i);
            else
                if (rem(i,6)==0)
                    y(3)=readval(i);
                end
            end
        end
    end
    if ( rem(i,6) == 0 )
        x(4) = x (1);
        y(4) = y (1);
        myhandles.plots=[ myhandles.plots plot(x,y,'r') ];
    end
end
end
guidata(gcbo,myhandles);

function plus_in(hObject, handles,i)
myhandles = guidata(gcbo);
myhandles.ver_in(i)=myhandles.ver_in(i)+1;

temp=myhandles.poly.getPosition;%flipud( handles.poly.getPosition );
tempx=temp(:,1);
tempy=temp(:,2);

for i=1:numel(tempx)
    myhandles.ver_in_plots(i)=plot(tempx(i),tempy(i));
    myhandles.hcmenu(i)=uicontextmenu;
    item1 = uimenu(myhandles.hcmenu(i), 'Label', sprintf('%i',myhandles.ver_in(i)));
    item2 = uimenu(myhandles.hcmenu(i), 'Label', '+','callback',{@plus_in,i});
    item3 = uimenu(myhandles.hcmenu(i), 'Label', '-','callback',{@minus_in,i});
    set(myhandles.ver_in_plots(i), 'uicontextmenu', myhandles.hcmenu(i));

end

temp=myhandles.poly.getPosition;
fid = fopen('out.txt','w');
fprintf(fid, '%i\r\n' , length(temp));
for i=1:length(temp)
    fprintf(fid ,'%f %f %i\r\n' , temp(i,:),myhandles.ver_in(i));
end
fclose(fid);

guidata(gcbo,myhandles);

function minus_in(hObject, handles,i)
myhandles = guidata(gcbo);
if myhandles.ver_in(i)==0
    return
end
myhandles.ver_in(i)=myhandles.ver_in(i)-1;

temp=myhandles.poly.getPosition;%flipud( handles.poly.getPosition );
tempx=temp(:,1);
tempy=temp(:,2);

for i=1:numel(tempx)
    myhandles.ver_in_plots(i)=plot(tempx(i),tempy(i));
    myhandles.hcmenu(i)=uicontextmenu;
    item1 = uimenu(myhandles.hcmenu(i), 'Label', sprintf('%i',myhandles.ver_in(i)));
    item2 = uimenu(myhandles.hcmenu(i), 'Label', '+','callback',{@plus_in,i});
    item3 = uimenu(myhandles.hcmenu(i), 'Label', '-','callback',{@minus_in,i});
    set(myhandles.ver_in_plots(i), 'uicontextmenu', myhandles.hcmenu(i));

end

temp=myhandles.poly.getPosition;
fid = fopen('out.txt','w');
fprintf(fid, '%i\r\n' , length(temp));
for i=1:length(temp)
    fprintf(fid ,'%f %f %i\r\n' , temp(i,:),myhandles.ver_in(i));
end
fclose(fid);

guidata(gcbo,myhandles);

  


% --- Executes on button press in simplify_button.
function simplify_button_Callback(hObject, eventdata, handles)
% hObject    handle to simplify_button (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

if handles.flag==1
    set(handles.poly,'Visible','off')
    for i=1:numel(handles.plots)
        set(handles.plots(i),'Visible','off')
    end
    for i=1:numel(handles.ver_in_plots)
        set(handles.ver_in_plots(i),'Visible','off')
    end
end

if handles.simp_flag == 1
    for i=1:numel(handles.simPlots)
        set(handles.simPlots,'Visible','off')
    end
end

handles.simp_flag=1;

fid = fopen('outSimple.txt','r');
loop_flag = 1;
rsize = 0;
[readval rsize] = fscanf(fid,'%f');
counter = 1;

polyx=[];
polyy=[];
while (loop_flag == 1)
    for i=1:rsize
        if ( rem(i,2) == 1 )
            polyx = [ polyx readval(i) ];
        else
            polyy = [ polyy readval(i) ];
        end
    end 
        polyx = [ polyx polyx(1) ];
        polyy = [ polyy polyy(1) ];
        
        handles.simPlots = [ handles.simPlots plot(polyx,polyy,'m') ];
        polyx=[];
        polyy=[];
        
        readval = fscanf(fid,'%s',1);
        if ( readval == '%' )
            loop_flag = 0;
        else
            [readval rsize] = fscanf(fid,'%f');
            counter=counter+1;
        end
end

fclose(fid);
guidata(hObject, handles);
    


% --- Executes on button press in show_source_butt.
function show_source_butt_Callback(hObject, eventdata, handles)
% hObject    handle to show_source_butt (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
 temp=handles.poly.getPosition;
 plot(temp);
 guidata(hObject, handles);
