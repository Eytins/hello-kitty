import 'package:flutter/material.dart';
import 'package:hellokitty/model/feeding_data.dart';
import '../components/custom_appbar.dart';
import '../components/path_config.dart';

class FeedingRecords extends StatelessWidget {
  FeedingRecords({super.key});
  final records = FeedingRecordData().records;

  final icons = [Icons.ac_unit, Icons.access_alarm, Icons.access_time];
  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: CustomAppBar(
        backgroudColor: Colors.black,
        title: "Feeding Records",
        height: 40,
        iconPath: IconPath.customOptions[3][1],
      ),
      body: ListView.builder(
          itemCount: records.length,
          itemBuilder: (context, index) {
            return Card(
                child: ListTile(
              title: Text(records[index].feedingTime),
              subtitle:
                  Text("Type: " + records[index].note + ", Food Weights: 5g"),
              leading: Icon(
                Icons.done,
                color: Colors.green,
              ),
              // trailing: Icon(Icons.access_time)
            ));
          }),
    );
  }
}
