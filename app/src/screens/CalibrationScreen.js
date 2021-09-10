import React, { useState, useEffect } from 'react';
import { View, StyleSheet, Button, Image, Text, TouchableOpacity } from 'react-native';
import Icon from 'react-native-vector-icons/FontAwesome';
import { colors } from '../assets/colors'
import { LogBox } from 'react-native';

import BluetoothSerial from "react-native-bluetooth-serial";

const CalibrationScreen = props => {
    
    LogBox.ignoreLogs([
        'Non-serializable values were found in the navigation state', ' Can\'t perform a React state update on an unmounted component.'
        ]);
    
    const [connected, setConnected] = useState(false);
    const [errorMessage, setErrorMessage] = useState('');
    const [isCalibrated, setIsCalibrated] = useState(false);

    const [instructionMessage, setInstructionMessage] = useState('aqui você ajusta os sensores à sua mão');
    const [progressMessage, setProgressMessage] = useState('aguarde...');
    const [messageColor, setMessageColor] = useState(colors.alert);
    const [showButton, setShowButton] = useState(true);
    const [buttonText, setButtonText] = useState('voltar');
    
    const btManager = BluetoothSerial// props.route.params.manager;
    const callback = props.route.params.callback;

    useEffect(() => {
        connectToGlove();
        lostConnection();
    }, [])

    const connectToGlove = async () => {
        console.log('connect to glove')
        const hc = await btManager.connect('3C:61:05:12:67:32') //ESP
        // const hc = await btManager.connect('98:D3:71:FD:5A:D7') //HC06
            .then(res => {
                console.log('Conectado com sucesso! ' + res.message)    
                setConnected(true);  
                readMessages(); 
            })
            .catch(err => {
                console.log('Deu ruim pra conectar! ' + err.message)
                setConnected(false)
                setErrorMessage('Não foi possível se conectar à luva')
            }) 
    };
    
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
                var dados = message['data'].replace(/(\r\n|\n|\r)/gm, "");
                if (dados != 'error') {
                    setMessageColor(colors.success)
                    setProgressMessage('calibragem finalizada com sucesso :)')
                    setButtonText('ok')
                    setIsCalibrated(true)
                }
                else {
                    setMessageColor(colors.error)
                    setProgressMessage('ocorreu um erro :( tente novamente')
                    setButtonText('voltar')
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

    return (
        <View style={styles.screen}>

            <View style={styles.container}>

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
                                        connectToGlove()
                                    }}>
                                    <Text style={[styles.textButtonNegative, {fontSize: 18, paddingBottom: 0}]}>conectar</Text>
                                </TouchableOpacity>
                            </View>
                        : console.log()}
                </View>

                <View style={styles.instructionContainer}>
                    <Text style={styles.text}>{instructionMessage}</Text>
                </View>

                <View style={styles.imageContainer}>
                    <Image
                        style={styles.image}
                        source={require('../assets/calibragem.png')}
                    />
                </View>

                {showButton 
                ? 
                    <View>
                        <TouchableOpacity activeOpacity={0.4} onPress={() => {
                                console.log('começar')
                                writeMessages('1')
                                setInstructionMessage('feche e abra os dedos durante 20 segundos')
                                setShowButton(false)
                            }}>
                            <Text style={styles.textButtonNegative}>começar</Text>
                        </TouchableOpacity>
                    </View>
                :
                    <View style={styles.instructionContainer}>
                        <Text style={[styles.text, {color: messageColor}]}>{progressMessage}</Text>
                    </View>
                }
            </View>

            <View style={styles.buttonContainer}>
                <View style={styles.button}>
                    <TouchableOpacity activeOpacity={0.4} onPress={() => {
                            btManager.disconnect();
                            callback(isCalibrated);
                            props.navigation.pop();
                        }}>
                        <Text style={styles.textButton}>{buttonText}</Text>
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
        alignItems: 'center'
    },
    container: {
        flex: 1,
        justifyContent: 'space-around',
        alignItems: 'center',
        width: '100%',
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
    instructionContainer: {
        width: '60%',
    },
    text: {
        color: colors.basePurple,
        paddingVertical: 30,
        textAlign: 'center',
        fontWeight: 'bold',
        fontSize: 24,
    },
    buttonContainer:{
        flexDirection: 'row',
        justifyContent: 'space-around',
    },
    button: {
        width: '100%',
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
    image: {
        width: '100%',
        height: '100%'
    },
    imageContainer: {
        flex: 1,
        alignContent: 'center',
        justifyContent: 'center',
        width: '50%',
        maxHeight: '30%',
    }
});

export default CalibrationScreen