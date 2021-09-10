import React, { useState, useEffect } from 'react';
import type {Node} from 'react';

import BluetoothSerial from "react-native-bluetooth-serial";
import Tts from 'react-native-tts';
import Navigator from './src/navigation/Navigator'

const App: () => Node = () => {

  const [btStatus, setBluetoothStatus] = useState(false);
  const [isConnected, setIsConnected] = useState(false);
  const [paired, setPaired] = useState('');
  const [messages, setMessages] = useState('start');
  const btManager = BluetoothSerial;

  const getStatusFromDevice = async () => {
    console.log('get status')
    const status = await btManager.isEnabled();
    console.log(status)
    setBluetoothStatus(status);
  };

  const getPairedList = async () => {
    console.log('paired list')
    const devices = await btManager.list();
    console.log(devices)
    setPaired(devices[0]['name'])

    const hc = await btManager.connect('98:D3:71:FD:5A:D7');
    console.log(hc)
    console.log('conectado?')

    const con = await btManager.isConnected();
    console.log(con);
    setIsConnected(con);
    readMessages()
  }

  const readMessages = async () => {
    console.log('read messages')
    
    BluetoothSerial.withDelimiter("\r\n").then(res => {
      console.log("delimiter setup", res);   
      btManager.on('read', message => {
        console.log('bt lendo msg')
        console.log(message['data'])
        setMessages(message['data']);
      });     
      });
    
  }

  useEffect(() => {
    console.log('use effect')
    // getStatusFromDevice();
    // getPairedList()
    // btManager.on("bluetoothEnabled", () => {
    //   console.log('bt is enabled');
    //   setBluetoothStatus(true);
    // });
    // btManager.on("bluetoothDisabled", () => {
    //   console.log('bt is not enabled');
    //   setBluetoothStatus(false);
    // });

    Tts.setDefaultLanguage('pt-BR');
    // Tts.setDefaultRate(0.6);
    Tts.addEventListener('tts-start', event => console.log('start', event));
    Tts.addEventListener('tts-finish', event => console.log('finish', event));
    Tts.addEventListener('tts-cancel', event => console.log('cancel', event));
  }, []);

  const speak = () => {
    console.log('speak')
    Tts.stop();
    Tts.speak('oi pai o que voce ta fazendo');
  }

  return (
    <Navigator />
  );
};

export default App;

// {"navigation": 
//   {"addListener": [Function addListener], 
//     "canGoBack": [Function canGoBack],
//      "dangerouslyGetParent": [Function dangerouslyGetParent], 
//      "dangerouslyGetState": [Function anonymous], 
//      "dispatch": [Function dispatch], 
//      "goBack": [Function anonymous], 
//      "isFocused": [Function isFocused], 
//      "navigate": [Function anonymous], 
//      "pop": [Function anonymous], 
//      "popToTop": [Function anonymous], 
//      "push": [Function anonymous], 
//      "removeListener": [Function removeListener],
//       "replace": [Function anonymous], 
//       "reset": [Function anonymous], 
//       "setOptions": [Function setOptions], 
//       "setParams": [Function anonymous]}, 
//   "route": {"key": "Talk-_TnBxSggd9qhDilAOTDXU", "name": "Talk", "params": {"teste": "teste"}}}