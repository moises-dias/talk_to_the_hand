import React, { useState, useEffect } from 'react';
import { View, StyleSheet, Button, Image, Text, TouchableOpacity } from 'react-native';
import { colors } from '../assets/colors'
import Icon from 'react-native-vector-icons/FontAwesome';

import BluetoothSerial from "react-native-bluetooth-serial";

const HomeScreen = props => {
    
    const btManager = BluetoothSerial;
    
    const [connected, setConnected] = useState(false);
    const [errorMessage, setErrorMessage] = useState('');
    const [isCalibrated, setIsCalibrated] = useState(false);

    useEffect(() => {
        connectToGlove(true);
        lostConnection();
    }, [])
    
    const connectToGlove = async firstTime => {
        console.log('connect to glove')
        const hc = await btManager.connect('3C:61:05:12:67:32') //ESP
        // const hc = await btManager.connect('98:D3:71:FD:5A:D7') //HC06
            .then(res => {
                console.log('Conectado com sucesso! ' + res.message) 
                console.log(btManager.isConnected())   
                setConnected(true); 
                setErrorMessage(res.message);
                if (firstTime) {
                    writeMessages('2');
                    readMessages();
                }
            })
            .catch(err => {
                console.log('Deu ruim pra conectar! ' + err.message)
                setConnected(false)
                setErrorMessage('Não foi possível se conectar à luva')
            })
    };

    const checkIsConnected = () => {
        btManager.isConnected()
            .then(message => {
                console.log('then')
                console.log(message)   
                setConnected(message); 
            })
            .catch(message => {
                console.log('catch')
                console.log(message)
                setConnected(false)
            })
    }
    
    const lostConnection = async () => {
        console.log('subscribe to lost connection')
        btManager.on('connectionLost', e => {
            console.log("deconectou " + e)
            setConnected(false)
            setErrorMessage('A conexão foi interrompida')
        });
        btManager.on('error', (err) => {
            console.log(`Error: ${err.message}`)
        })
    }

    const readMessages = async () => {
        console.log('read messages')

        btManager.withDelimiter("\r\n").then(res => {
            console.log("delimiter setup", res); 
            btManager.on('read', message => {
                console.log('recebendo mensagem')
                console.log(message)
                var dados = message['data'].replace(/(\r\n|\n|\r)/gm, "");
                if (dados == 'true') {
                    setIsCalibrated(true)
                }
                else if (dados == 'false') {
                    setIsCalibrated(false)
                }
            });     
        });        
    }
    
    const writeMessages = async message => {
        btManager.write(message)
        .then((res) => {
            console.log('Successfuly wrote to device: ' + message)
        })
        .catch((err) => console.log(err.message))
    }

    const callbackPop = calibrated => {
        console.log('callback! ' + calibrated)
        connectToGlove(false);
        setIsCalibrated(calibrated);
    }

    return (
        <View style={styles.screen}>

            <View style={styles.connectionContainer}>
                <View style={styles.messageContainer}>
                    <Icon 
                        name="circle" 
                        size={10} 
                        color={connected ? colors.success : colors.error}
                    />
                    <Text style={connected ? styles.connected : styles.disconnected}> {connected ? 'conectado' : 'desconectado'}</Text>                            
                </View>
                {!connected ? 
                    <View style={styles.conectarContainer}>                            
                        <Text style={[styles.disconnected, {paddingVertical: 10}]}>{errorMessage}</Text>
                        <TouchableOpacity activeOpacity={0.4} onPress={() => {
                                console.log('CONECTAR')
                                connectToGlove(false)
                            }}>
                            <Text style={[styles.textButtonNegative, {fontSize: 18, paddingBottom: 0}]}>conectar</Text>
                        </TouchableOpacity>
                    </View>
                : console.log()}
            </View>

            <View style={styles.imageContainer}>
                <Image
                    style={styles.image}
                    source={require('../assets/logo.png')}
                />
            </View>
            <View style={styles.buttonContainer}>
                <View style={styles.button}>
                    <TouchableOpacity activeOpacity={0.4} disabled={!isCalibrated} onPress={() => {
                            btManager.disconnect();
                            props.navigation.push('Talk', {callback: callbackPop});
                        }}>
                        <Text style={[styles.textButton, isCalibrated ? { backgroundColor : colors.basePurple} : {backgroundColor: colors.disabled}]}>falar</Text>
                    </TouchableOpacity>
                </View>
                <View style={styles.button}>
                    <TouchableOpacity activeOpacity={0.4} onPress={() => {
                            btManager.disconnect();
                            props.navigation.push('Calibration', {callback: callbackPop});
                        }}>
                        <Text style={styles.textButton}>calibrar</Text>
                    </TouchableOpacity>
                </View>
            </View>
        </View>
    )
};

const styles = StyleSheet.create({
    screen: {
        flex: 1,
        justifyContent: 'space-between',
        alignItems: 'center',
        borderColor: "black",
    },
    connectionContainer: {
        justifyContent: 'flex-start',
    },
    conectarContainer: {
        justifyContent: 'flex-end'
    },
    messageContainer: {
        flexDirection: 'row',
        justifyContent: 'center',
        alignItems: 'center',
        marginTop: 20,
    },
    connected: {
        color: colors.success,
        textAlign: 'center',
        fontWeight: 'bold',
        fontSize: 18,
    },
    disconnected: {
        color: colors.error,
        textAlign: 'center',
        fontWeight: 'bold',
        fontSize: 18,
    },
    image: {
        width: '100%',
        height: '50%',
    },
    imageContainer: {
        flex: 1,
        alignContent: 'center',
        justifyContent: 'center',
        width: '70%',
    },
    buttonContainer:{
        flexDirection: 'row',
        justifyContent: 'space-around',
    },
    button: {
        width: '50%',
    },
    textButton: {
        color: 'white',
        paddingVertical: 30,
        textAlign: 'center',
        fontWeight: 'bold',
        fontSize: 18,
        backgroundColor: colors.basePurple,
        borderWidth: 1,
        borderRadius: 6,
        borderColor: "white",
    },
    textButtonNegative: {
        color: colors.basePurple,
        paddingBottom: 30,
        textAlign: 'center',
        fontWeight: 'bold',
        fontSize: 26,
    },
});

export default HomeScreen