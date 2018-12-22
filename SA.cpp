#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string>

using namespace std;

typedef struct {
  int capacity;
  int cost;
  int occupy;
}facility;

typedef struct {
  int demand;
  int *cost;
  int facilityID;
}customer;

bool accept(int difference, float temperature){
  return exp((-difference) / temperature) >= (double)(rand() % 1000 / 1000.0);
}

void SA(string file){
  //读取文件
  ifstream infile("./Instances/p" + file); 
  ofstream outfile("./SA_output", ios::app);
  int facilityNum, customerNum;
  float temp;
  infile >> facilityNum >> customerNum;
  facility *facilities = new facility[facilityNum];
  customer *customers = new customer[customerNum];
  for (int i = 0; i < facilityNum; i++){
    infile >> facilities[i].capacity >> facilities[i].cost;
    facilities[i].occupy = 0;
  }
  for (int i = 0; i < customerNum; i++){
    infile >> temp;
    customers[i].demand = (int) temp;
    customers[i].cost = new int[facilityNum];
    customers[i].facilityID = -1;
  }
  for (int i = 0; i < facilityNum; i++)
    for (int j = 0; j < customerNum; j++){
      infile >> temp;
      customers[j].cost[i] = (int) temp;
    }
  infile.close();

  //随机生成一个初态
  int facilityCost = 0, customerCost = 0;
  //为每个顾客随机分配一个工厂
  for (int i = 0; i < customerNum; i++){
    while (true){
      srand((unsigned)time(NULL));
      int randnum = rand() % facilityNum;
      //工厂有足够空间提供
      if (facilities[randnum].capacity - facilities[randnum].occupy >= customers[i].demand){
        //如果工厂没开工，计算开工费用
        if (facilities[randnum].occupy == 0)
          facilityCost += facilities[randnum].cost;
        facilities[randnum].occupy += customers[i].demand;
        customerCost += customers[i].cost[randnum];
        customers[i].facilityID = randnum;
        break;
      }
    }
  }

  //模拟退火
  float temperature = 100.0;
  while (temperature > 0.1){
    int count = 0;
    //每个温度下进行200次状态空间搜索
    while (count < 200){
      count++;
      //随机产生新状态
      int randcustomer = rand() % customerNum;
      int randfacility = 0;
      while (true){
        randfacility = rand() % facilityNum;
        if (randfacility != customers[randcustomer].facilityID &&
          facilities[randfacility].capacity - facilities[randfacility].occupy >= customers[randcustomer].demand)
          break;
      }
      //计算新状态的值
      int newFacilityCost = facilityCost;
      if (facilities[customers[randcustomer].facilityID].occupy == customers[randcustomer].demand)
        newFacilityCost -= facilities[customers[randcustomer].facilityID].cost;
      if (facilities[randfacility].occupy == 0)
        newFacilityCost += facilities[randfacility].cost;
      int newCustomerCost = customerCost 
                          - customers[randcustomer].cost[customers[randcustomer].facilityID] 
                          + customers[randcustomer].cost[randfacility];
      //判断是否接受新状态，如果接受就更新状态
      if (accept((newFacilityCost+ newCustomerCost) - (facilityCost + customerCost), temperature)){
        facilities[customers[randcustomer].facilityID].occupy -= customers[randcustomer].demand;
        facilities[randfacility].occupy += customers[randcustomer].demand;
        customers[randcustomer].facilityID = randfacility;
        facilityCost = newFacilityCost;
        customerCost = newCustomerCost;
      }
    }
    //降温
    temperature *= 0.98;
  }

  //输出文件
  outfile << "Test file p" + file + ": " << facilityCost + customerCost << endl;
  outfile.close();

  //释放内存空间
  for (int i = 0; i < customerNum; i++){
    delete []customers[i].cost;
  }
  delete []facilities;
  delete []customers;
}

int main(){
  for (int i = 1; i <= 71; i++){
    SA(to_string(i));
  }
  return 0;
}