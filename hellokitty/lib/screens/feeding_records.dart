import 'package:flutter/material.dart';
import 'package:hellokitty/model/feeding_records.dart';
import 'package:hellokitty/utils/api_service.dart';
import '../components/custom_appbar.dart';
import '../components/path_config.dart';

class FeedingRecords extends StatefulWidget {
  // const FeedingRecords({super.key});
  const FeedingRecords({Key? key}) : super(key: key);

  @override
  State<FeedingRecords> createState() => _FeedingRecordsState();
}

class _FeedingRecordsState extends State<FeedingRecords> {
  // var records = FeedingRecordData().records;
  List<FeedingRecordsData> records = [];
  @override
  void initState() {
    super.initState();
    getFeedingRecords();
  }

  void getFeedingRecords() async {
    ApiService apiService = ApiService();
    await apiService.getRecords().then((value) => setState(() {
          if (value != null) records = value;
        }));
    debugPrint('feeding records:  $records');
  }

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
              title: Text(records[index].feedDate),
              subtitle: Text("Type: " +
                  records[index].type.toString() +
                  ", Food Weights: " +
                  records[index].foodWeight.toString()),
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

// class FeedingRecords extends StatelessWidget {
//   // FeedingRecords({super.key});
//   FeedingRecords({Key? key}) : super(key: key);
//   final records = FeedingRecordData().records;

//   @override
//   Widget build(BuildContext context) {
//     return Scaffold(
//       appBar: CustomAppBar(
//         backgroudColor: Colors.black,
//         title: "Feeding Records",
//         height: 40,
//         iconPath: IconPath.customOptions[3][1],
//       ),
//       body: ListView.builder(
//           itemCount: records.length,
//           itemBuilder: (context, index) {
//             return Card(
//                 child: ListTile(
//               title: Text(records[index].feedingTime),
//               subtitle:
//                   Text("Type: " + records[index].note + ", Food Weights: 5g"),
//               leading: Icon(
//                 Icons.done,
//                 color: Colors.green,
//               ),
//               // trailing: Icon(Icons.access_time)
//             ));
//           }),
//     );
//   }
// }
