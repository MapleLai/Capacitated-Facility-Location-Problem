#include <iostream>
#include <fstream>

using namespace std;

typedef struct {
  int capacity;
  int cost;
  int occupy;
}facility;

typedef struct {
  int demand;
  int *cost;
  int *costID;
}customer;

void greedy(string file){
  //读取文件
  ifstream infile("./Instances/p" + file); 
  ofstream outfile("./Greedy_output", ios::app);
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
    customers[i].costID = new int[facilityNum];
  }
  for (int i = 0; i < facilityNum; i++)
    for (int j = 0; j < customerNum; j++){
      infile >> temp;
      customers[j].cost[i] = (int) temp;
      customers[j].costID[i] = i;
    }
  infile.close();

  //将每个顾客的工厂cost按升序排序
  for (int i = 0; i < customerNum; i++){
    int tempp;
    for (int j = 0; j < facilityNum - 1; j++)
      for (int k = j + 1; k < facilityNum; k++)
        if (customers[i].cost[j] > customers[i].cost[k]){
          tempp = customers[i].cost[j];
          customers[i].cost[j] = customers[i].cost[k];
          customers[i].cost[k] = tempp;
          tempp = customers[i].costID[j];
          customers[i].costID[j] = customers[i].costID[k];
          customers[i].costID[k] = tempp;
        }
  }

  //将顾客依照第一志愿工厂的cost按升序排序
  for (int i = 0; i < customerNum - 1; i++){
    customer tempp;
    for (int j = 0; j < customerNum; j++)
      if (customers[i].cost[0] > customers[j].cost[0]){
        tempp = customers[i];
        customers[i] = customers[j];
        customers[j] = tempp;
      }
  }

  //给每个顾客分配工厂
  int facilityCost = 0, customerCost = 0;
  for (int i = 0; i < customerNum; i++){
    int count = 0;
    while (count < facilityNum){
      int facilityID = customers[i].costID[count];
      if (facilities[facilityID].capacity - facilities[facilityID].occupy > customers[i].demand){
        if (facilities[facilityID].occupy == 0)
          facilityCost += facilities[facilityID].cost;
        customerCost += customers[i].cost[count];
        facilities[facilityID].occupy += customers[i].demand;
        break;
      }
      count++;
    }
  }

  //输出文件
  outfile << "Test file p" + file + ": "   << facilityCost + customerCost << endl;
  outfile.close();

  //释放内存空间
  for (int i = 0; i < customerNum; i++){
    delete []customers[i].cost;
    delete []customers[i].costID;
  }
  delete []facilities;
  delete []customers;
}

int main(){
  for (int i = 1; i <= 71; i++){
    greedy(to_string(i));
  }
  return 0;
}