#include "GPSFormat.h"
#include <stdio.h>

latLong GPSFormat::gpsLocation(String local)
{
  
  while(1)
  {
    int i=0;
    while(local[i]!=':')i++;

    String loc_2 = local.substring(i+2);

    i=0;
    while(loc_2[i]!=',')i++;

    String lat = loc_2.substring(0,i);
    SerialUSB.print("LAT: ");
    SerialUSB.println(lat);

    int j = i+3;
    int k = j;

    while(loc_2[k]!=',')k++;

    String lon = loc_2.substring(j,k);
    SerialUSB.print("LONG: ");
    SerialUSB.println(lon); 
    
    latLong values;
    values.lattitude = lat;
    values.longitude = lon; 

    return values;
    
  }
}
String GPSFormat::conversion(String local, int i)
{
  String str_end;
  String str_init;
  if(local[0]=='0'){
      SerialUSB.print("local = 0\r\n");
      str_end = local.substring(3);
      str_init = local.substring(1,3);
      int val = str_init.toInt();
      double deci_val = str_end.toDouble();
      double new_value = val+(deci_val / 60);
      
      if(i == 1)
      {
        new_value = new_value * 10;
      }     

      String final_val = String(new_value,5);

      return final_val;
   }
  else if(local[0]!='0')
  {
      SerialUSB.print("local != 0\r\n");
      if(i == 0)
      {
        str_end = local.substring(2);
        str_init = local.substring(0,2);
      } 
      else if(i == 1)
      {
        str_end = local.substring(3);
        str_init = local.substring(0,3);
      }

      int val = str_init.toInt();
      double deci_val = str_end.toDouble(); 
      double new_value = val+(deci_val / 60); 
      String final_val = String(new_value,5);

      return final_val;
  }
}
