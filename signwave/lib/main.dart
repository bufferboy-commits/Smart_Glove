import 'package:firebase_core/firebase_core.dart';
import 'package:firebase_database/firebase_database.dart';
import 'package:flutter/material.dart';
import 'package:flutter_tts/flutter_tts.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Firebase.initializeApp();

  // Get the FirebaseApp instance
  FirebaseApp app = Firebase.app();

  // Pass the app instance and the correct URL to FirebaseDatabase
  final databaseReference = FirebaseDatabase.instanceFor(
    app: app,
    databaseURL: "https://iot-glove-77273-default-rtdb.asia-southeast1.firebasedatabase.app",
  ).ref();

  runApp(MyApp(databaseReference)); // Pass the database reference to the app
}

class MyApp extends StatelessWidget {
  final DatabaseReference databaseReference;
  MyApp(this.databaseReference);

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'SignWave',
      theme: ThemeData(
        primarySwatch: Colors.indigo,
      ),
      home: GestureListeningScreen(databaseReference),
    );
  }
}

class GestureListeningScreen extends StatefulWidget {
  final DatabaseReference databaseReference;
  GestureListeningScreen(this.databaseReference);

  @override
  _GestureListeningScreenState createState() => _GestureListeningScreenState();
}

class _GestureListeningScreenState extends State<GestureListeningScreen> {
  final FlutterTts flutterTts = FlutterTts();
  String lastSpokenGesture = '';
  String currentGesture = 'Listening...';

  @override
  void initState() {
    super.initState();
    _listenForGestureChanges();
  }

  void _listenForGestureChanges() {
    widget.databaseReference.child('gestures/gesture1').onValue.listen((event) {
      final snapshot = event.snapshot;
      if (snapshot.exists) {
        final newGesture = snapshot.child('name').value.toString();
        if (newGesture != lastSpokenGesture) {
          setState(() {
            lastSpokenGesture = newGesture;
            currentGesture = newGesture;
          });
          _speakGesture(newGesture);
        }
      } else {
        setState(() {
          currentGesture = 'No data available';
        });
        print("No data available at gestures/gesture1");
      }
    }, onError: (error) {
      print("Error listening to Firebase: $error");
    });
  }

  Future<void> _speakGesture(String gesture) async {
    try {
      await flutterTts.setLanguage('en-US');
      await flutterTts.setPitch(1.0);
      await flutterTts.setSpeechRate(0.5);
      await flutterTts.speak(gesture);
      print("Spoken Gesture: $gesture");
    } catch (e) {
      print("Error in TTS: $e");
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('SignWave', style: TextStyle(color: Colors.white),),
        centerTitle: true,
        elevation: 2,
        // backgroundColor: Colors.black,
        flexibleSpace: Container(
          decoration: BoxDecoration(
            gradient: LinearGradient(
              colors: [Colors.indigo, Colors.blueAccent], // Gradient colors
              begin: Alignment.topLeft,
              end: Alignment.bottomRight,
            ),
         ),
        ),
      ),
      body: Container(
        decoration: BoxDecoration(
          gradient: LinearGradient(
            colors: [Colors.indigo, Colors.blueAccent],
            begin: Alignment.topLeft,
            end: Alignment.bottomRight,
          ),
        ),
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // Replace the icon with an image
              ClipRRect(
                borderRadius: BorderRadius.circular(12),
                child: Image.asset(
                  'assets/glove_image.jpg', // Path to your local image
                  width: 150,
                  height: 150,
                  fit: BoxFit.cover,
                ),
              ),
              SizedBox(height: 20),
              AnimatedContainer(
                duration: Duration(milliseconds: 500),
                padding: EdgeInsets.all(20),
                decoration: BoxDecoration(
                  color: Colors.white.withOpacity(0.9),
                  borderRadius: BorderRadius.circular(12),
                  boxShadow: [
                    BoxShadow(
                      color: Colors.black26,
                      blurRadius: 10,
                      offset: Offset(0, 5),
                    ),
                  ],
                ),
                child: Column(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    Text(
                      "Current Gesture:",
                      style: TextStyle(
                        fontSize: 22,
                        fontWeight: FontWeight.w500,
                        color: Colors.teal[800],
                      ),
                    ),
                    SizedBox(height: 10),
                    Text(
                      currentGesture,
                      style: TextStyle(
                        fontSize: 32,
                        fontWeight: FontWeight.bold,
                        color: Colors.teal,
                      ),
                      textAlign: TextAlign.center,
                    ),
                  ],
                ),
              ),
              SizedBox(height: 40),
              CircularProgressIndicator(
                valueColor: AlwaysStoppedAnimation<Color>(Colors.white),
              ),
              SizedBox(height: 20),
              Text(
                'Waiting for new gesture...',
                style: TextStyle(
                  fontSize: 16,
                  color: Colors.white,
                  fontStyle: FontStyle.italic,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
