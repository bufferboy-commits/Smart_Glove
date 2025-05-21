import 'package:flutter/material.dart';
import 'package:firebase_core/firebase_core.dart';

class TestFirebaseScreen extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return FutureBuilder(
      future: Firebase.initializeApp(),
      builder: (context, snapshot) {
        if (snapshot.connectionState == ConnectionState.done) {
          return Scaffold(
            appBar: AppBar(title: Text('Firebase Connected')),
            body: Center(child: Text('Firebase is working!')),
          );
        }
        if (snapshot.hasError) {
          return Scaffold(
            appBar: AppBar(title: Text('Error')),
            body: Center(child: Text('Error initializing Firebase')),
          );
        }
        return Scaffold(
          appBar: AppBar(title: Text('Loading')),
          body: Center(child: CircularProgressIndicator()),
        );
      },
    );
  }
}
