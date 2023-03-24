import 'package:flutter/material.dart';
import '../components/custom_appbar.dart';
import '../components/path_config.dart';

class FeedingRecords extends StatelessWidget {
  const FeedingRecords({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: CustomAppBar(
        backgroudColor: Colors.black,
        title: "Feeding Records",
        height: 40,
        iconPath: IconPath.customOptions[3][1],
      ),
      body: Center(
        child: ElevatedButton(
          onPressed: () {
            Navigator.pop(context);
          },
          child: const Text('Go back!'),
        ),
      ),
    );
  }
}
