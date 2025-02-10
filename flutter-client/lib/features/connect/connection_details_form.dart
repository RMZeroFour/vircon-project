import 'package:flutter/material.dart';

class ConnectionDetailsForm extends StatefulWidget {
  final String? initialHost;
  final String? initialPort;

  final void Function(String host, int port) onConnect;

  const ConnectionDetailsForm({
    super.key,
    this.initialHost,
    this.initialPort,
    required this.onConnect,
  });

  @override
  State<ConnectionDetailsForm> createState() => ConnectionDetailsFormState();
}

class ConnectionDetailsFormState extends State<ConnectionDetailsForm> {
  final GlobalKey<FormState> _formKey = GlobalKey<FormState>();

  late final TextEditingController _hostController;
  late final TextEditingController _portController;

  @override
  void initState() {
    super.initState();

    _hostController = TextEditingController(text: widget.initialHost);
    _portController = TextEditingController(text: widget.initialPort);
  }

  @override
  void dispose() {
    _hostController.dispose();
    _portController.dispose();

    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return Form(
      key: _formKey,
      child: Column(
        children: <Widget>[
          TextFormField(
            controller: _hostController,
            decoration: InputDecoration(
              hintText: 'Server Host',
            ),
            validator: (value) =>
                _isValidHost(value) ? null : 'Please enter the server host',
          ),
          TextFormField(
            controller: _portController,
            keyboardType: TextInputType.number,
            decoration: InputDecoration(
              hintText: 'Server Port',
            ),
            validator: (value) =>
                _isValidPort(value) ? null : 'Please enter the server port',
          ),
          ElevatedButton(
            onPressed: () {
              if (_formKey.currentState!.validate()) {
                final int port = int.parse(_portController.text);
                widget.onConnect(_hostController.text, port);
              }
            },
            child: Text('Connect'),
          ),
        ],
      ),
    );
  }

  bool _isValidHost(String? value) {
    return value != null && value.isNotEmpty;
  }

  bool _isValidPort(String? value) {
    if (value != null) {
      final int? port = int.tryParse(value);
      return (port != null && port >= 0 && port < 65536);
    }
    return false;
  }
}
