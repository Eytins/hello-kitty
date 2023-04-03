class FeedingRecord {
  final String feedingTime;
  final String note;

  FeedingRecord({required this.feedingTime, required this.note});
}

class FeedingRecordData {
  List<FeedingRecord> records = [
    FeedingRecord(feedingTime: "2023-04-02 12:03", note: "Auto"),
    FeedingRecord(feedingTime: "2023-04-02 18:31", note: "Auto"),
    FeedingRecord(feedingTime: "2023-03-25 08:33", note: "Manual"),
    FeedingRecord(feedingTime: "2023-03-25 12:30", note: "Auto"),
    FeedingRecord(feedingTime: "2023-03-25 18:31", note: "Auto"),
  ];
}
