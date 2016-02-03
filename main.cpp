#include<iostream>
#include<map>
#include<vector>
#include<algorithm>
#include<cmath>
#include<climits>
#include<ctime>
#include<cstring>
#include<stack>
#include<queue>
#include<sstream>
#include<string>
#include<cassert>
#include<unordered_map>
#include<fstream>
#include<set>
#include<time.h> 

#define ALL(v) (v).begin(),(v).end()
#define REP(i,p,n) for(int i=p;i<(int)(n);++i)
#define rep(i,n) REP(i,0,n)
#define DUMP(list) cout << "{ "; for(auto nth : list){ cout << nth << " "; } cout << "}" << endl

using namespace std;

struct node{
  int color;
  int residual;
  int saturation;

  node(){
    color=0;residual=0;saturation=0;
  }
};


const string MARGEDIC[3]={"0x0",
                    "x11",
                    "011"};

vector<string> t; //テストパターン
vector<string> dic;
vector<node> nodes;
vector<vector<bool>> graph;
vector<vector<int>> nodesColor(1500);

template<class T> inline string toString(T x) {ostringstream sout;sout<<x;return sout.str();}

vector<string> input(string fname){
  vector<string> tmp; 
  ifstream ifs(fname);
  string str;
  if (ifs.fail()){
    cerr << "入力ファイルをオープンできません" << endl;
    exit(0);
  }
  while (getline(ifs, str)){
    if(str[str.size()-1]=='\r') str=str.substr(0,str.size()-1);
    tmp.push_back(str);
  }
 
  return tmp;
}

void output(string fname,vector<string> texts){
  ofstream fout(fname); 
  
  if(fout.fail()){ 
    cout << "出力ファイルをオープンできません" << endl;
    exit(1);
  }
  
  rep(i,texts.size()){
    fout << texts[i] << endl;
  }
}

vector<vector<bool>> init(){
 
  int wc=t.size();

  vector<vector<bool>> t_map(wc,vector<bool>(wc,false));//マージできるものはtrue
  rep(i,wc) nodes.push_back(node());
  int len=t[0].size();

  rep(i,t.size()){
    rep(j,t.size()){  
      bool flag=true;
      rep(k,len-1){
        int p1=-1,p2=-1;
        if(t[i][k]=='X') p1=2;
        else p1=t[i][k]-'0'; 

        if(t[j][k]=='X') p2=2;
        else p2=t[j][k]-'0';

        if(MARGEDIC[p1][p2]=='x'){
          flag=false;
          nodes[i].residual++;
          nodes[j].residual++;
          break;
        }
      }
      if(flag){
        t_map[i][j]=true;
        t_map[j][i]=true;  
      }
    }
  }
  return t_map;
}

vector<string> fairTp(){
  vector<string> tmp;
 
  rep(i,nodesColor.size()){
    string str="";
    if(nodesColor[i].size()!=0){
      str+="new_tp"+toString(i-1)+ " : "; 
      rep(j,nodesColor[i].size()){
        str+="tp"+toString(nodesColor[i][j]);
        if(j!=nodesColor[i].size()-1) str+=",";
      }
      tmp.push_back(str);
    }
  }

  return tmp;
}

vector<string> searchTpDic(int index){
  vector<string> tmpDic;
  int cnt=0;
 
  string tmpName="tp"+toString(index);

  rep(i,dic.size()){
    if(dic[cnt]==tmpName){
      while(1){
        cnt++;
        if(dic[cnt]!="") tmpDic.push_back(dic[cnt]);
        else return tmpDic;
      }
    }
    cnt++;
  }

  return tmpDic;
}

vector<string> fairDic(){
  vector<string> tmp;
  vector<set<string>> tmpDic(nodesColor.size()); //index番目の色はどのような辞書になっているか
  int max_col=0;
  
  rep(j,nodesColor.size()){
    rep(k,nodesColor[j].size()){
      vector<string> d=searchTpDic(nodesColor[j][k]);
      max_col=max(max_col,j);
      rep(i,d.size()){
        tmpDic[j].insert(d[i]);
      }
    }
  }

  rep(i,max_col+1){
    set<string>::iterator ite = tmpDic[i].begin();
    if(i==0)continue;
    tmp.push_back("new_tp"+toString(i-1));
    while(ite != tmpDic[i].end()){
      string str = *ite;
      tmp.push_back(str);
      ite++;
    }
    tmp.push_back("");
  }

  return tmp;
}

void calcSaturation(int num){//飽和次数
  vector<bool> colors(1500,false);
  int col_max=1,cnt=0;
  rep(i,graph[num].size()){
    if(!graph[num][i]){
      colors[nodes[i].color]=true; 
      col_max=max(col_max,nodes[i].color);
    }
  }
  rep(i,col_max+1){
   if(colors[i]) cnt++; 
  }
  nodes[num].saturation=cnt;
}

void calcResidual(int num){ //残余字数
  nodes[num].residual--;
}

void paint(int num){
  vector<bool> colors(1500,false);
  vector<int> saf_nodes;
  int col_max=1;
  colors[0]=true;
  rep(i,graph[num].size()){
    if(!graph[num][i]){
      colors[nodes[i].color]=true; 
      col_max=max(col_max,nodes[i].color);
      calcResidual(i);
      saf_nodes.push_back(i);
    }
  }

  rep(i,col_max+2){
    if(!colors[i]){
      nodes[num].color=i;
      nodesColor[i].push_back(num);
      break;
    }
  }
  rep(i,saf_nodes.size()){
    calcSaturation(saf_nodes[i]);
  }
}

void paintAll(){

  rep(num,nodes.size()){
    if(nodes[num].color!=0) continue;
    vector<bool> colors(1500,false);
    int col_max=1;
    colors[0]=true;
    rep(i,graph[num].size()){
      if(!graph[num][i]){
        colors[nodes[i].color]=true; 
        col_max=max(col_max,nodes[i].color);
      }
    }

    rep(i,col_max+2){
      if(!colors[i]){
        nodes[num].color=i; 
        nodesColor[i].push_back(num);
        break;
      }
    }
  }
}

int main(int argc,char *argv[]){

  
  vector<string> fnames(argc,"");
  rep(i,argc){
    string str = argv[i];
    fnames[i]=str;
  }

  int sTime=clock();
  
  t= input(fnames[1]);
  graph = init();

  while(1){ 
    int tmp_res=0,tmp_sat=0,coloring=0;
    rep(i,nodes.size()){
     if(nodes[i].color==0) tmp_res=max(tmp_res,nodes[i].residual); 
    }
    if(tmp_res==0) break; //終了条件
    rep(i,nodes.size()){
      if(nodes[i].residual==tmp_res && nodes[i].color==0){
        tmp_sat=max(tmp_sat,nodes[i].saturation);
        coloring=i;
      }
    }
    paint(coloring);//彩色
  }

  paintAll();

  int eTime=clock();

  cout << "SearchTime = " << (eTime-sTime)/CLOCKS_PER_SEC << "sec." << endl;

  cout << "Now data writing..." << endl;
 
  output(fnames[3],fairTp());
  dic=input(fnames[2]);
  output(fnames[4],fairDic());

  cout << "Completion of writing" << endl;

  return 0;
}
