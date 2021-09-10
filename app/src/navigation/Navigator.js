import React from 'react';
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator } from '@react-navigation/stack';
import 'react-native-gesture-handler';

import HomeScreen from './../screens/HomeScreen'
import TalkScreen from './../screens/TalkScreen'
import CalibrationScreen from './../screens/CalibrationScreen'
import ConnectionScreen from './../screens/ConnectionScreen'


const Navigator = props => {
    
  const Stack = createStackNavigator();
  
  const AppTheme = {
    colors: {
      background: 'white'
    },
  };

  const screenOptions = (title) => {
    return {
        title: title,
        headerStyle: {
        backgroundColor: '#300055',
        height: 80
        },
        headerTintColor: 'white',
        headerTitleStyle: {
          alignSelf: 'center',
          fontWeight: 'bold'
        },
        headerLeft: null //desabilita back button
    }
  }

    return (
        <NavigationContainer theme={ AppTheme } >
        <Stack.Navigator initialRouteName="Home">
          <Stack.Screen 
            name="Home"
            component={HomeScreen}
            options={screenOptions('home')}
          />
          <Stack.Screen
            name="Talk"
            component={TalkScreen}
            options={screenOptions('falar')}
          />
          <Stack.Screen
            name="Calibration"
            component={CalibrationScreen}
            options={screenOptions('calibrar')}
          />
          <Stack.Screen
            name="Connection"
            component={ConnectionScreen}
            options={screenOptions('conectar')}
          />
        </Stack.Navigator>
      </NavigationContainer>
    )
};

export default Navigator;