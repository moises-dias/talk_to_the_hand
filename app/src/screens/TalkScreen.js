import React, { useState, useEffect, useRef } from 'react';
import { View, StyleSheet, Button, Image, Text, TextInput, TouchableOpacity } from 'react-native';
import Icon from 'react-native-vector-icons/FontAwesome';
import { colors } from '../assets/colors'
import { LogBox } from 'react-native';

import BluetoothSerial from "react-native-bluetooth-serial";
import Tts from 'react-native-tts';

const TalkScreen = props => {

    LogBox.ignoreLogs([
        'Non-serializable values were found in the navigation state', ' Can\'t perform a React state update on an unmounted component.'
        ]);
    
    const btManager = BluetoothSerial//props.route.params.manager;
    const callback = props.route.params.callback;

    const [text, setText] = useState('');
    const [word, setWord] = useState('');
    
    const textRef = useRef('');
    const wordRef = useRef('');

    const [connected, setConnected] = useState(false);
    const [errorMessage, setErrorMessage] = useState('');

    useEffect(() => {
        connectToGlove();
        lostConnection();
        setupAudio();
    },[])

    const setupAudio = () => {
        Tts.setDefaultLanguage('pt-BR');
    }

    const speak = value => {
        Tts.stop();
        Tts.speak(value);
    }

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
        btManager.on('connectionLost', e => {
            console.log("deconectou " + e)
            setConnected(false)
            setErrorMessage('A conexão foi interrompida')
        });
        btManager.on('error', (err) => {
            console.log(`Error: ${err.message}`)
            // setConnected(false)
            // setErrorMessage(err.message)
        })
    }

    const readMessages = async () => {
        console.log('read messages')

        btManager.withDelimiter("\r\n").then(res => {
            console.log("delimiter setup", res); 

            btManager.on('read', message => {
                var dados = message['data'].replace(/(\r\n|\n|\r)/gm, "");
                console.log('dados: ' + dados)
                if (dados == '.') {
                    speak(wordRef.current)
                    const aux = textRef.current + wordRef.current + ' ';
                    textRef.current = aux
                    setText(aux)
                    wordRef.current = ''
                    setWord('')
                }
                else {
                    const aux = wordRef.current + dados
                    wordRef.current = aux
                    setWord(aux);
                }
            });     
        });        
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
                                    connectToGlove(false)
                                }}>
                                <Text style={[styles.textButtonNegative, {fontSize: 18, paddingBottom: 0}]}>conectar</Text>
                            </TouchableOpacity>
                        </View>
                    : console.log()}
                </View>

                <View style={[styles.textArea, connected ? { borderColor: colors.basePurple } : {borderColor: colors.error}]}>
                    <TextInput
                        multiline={true}
                        style={styles.text}
                        value={text}
                        onChangeText={t => setText(t)}
                    />
                </View>

                <View style={styles.icon}>
                    <TouchableOpacity activeOpacity={0.4} onPress={() => {
                            speak(text)
                        }}>
                        <Icon name="volume-up" size={30} color={colors.basePurple} />
                    </TouchableOpacity>
                </View>

                <View style={[styles.wordArea, connected ? { borderColor: colors.basePurple } : {borderColor: colors.error}]}>
                    <TextInput
                        style={styles.word}
                        value={word}
                        onChangeText={w => setWord(w)}
                    />
                </View>

                <View>
                    <TouchableOpacity activeOpacity={0.4} onPress={() => {
                            textRef.current = ''
                            setText('')
                        }}>
                        <Text style={styles.textButtonNegative}>limpar</Text>
                    </TouchableOpacity>
                </View>
            </View>

            <View style={styles.buttonContainer}>
                <View style={styles.button}>
                    <TouchableOpacity activeOpacity={0.4} onPress={() => {
                            btManager.disconnect();
                            callback(true);
                            props.navigation.pop();
                        }}>
                        <Text style={styles.textButton}>voltar</Text>
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
    },
    container: {
        flex: 1,
        justifyContent: 'flex-start',
        alignItems: 'center',
        width: '100%',
    },
    connectionContainer: {
        justifyContent: 'flex-start',
        marginBottom: 20
    },
    messageContainer: {
        flexDirection: 'row',
        justifyContent: 'center',
        alignItems: 'center',
        marginTop: 20,
    },
    conectarContainer: {
        justifyContent: 'flex-end'
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
    icon: {
        margin: 30
    },
    textArea: {
        borderWidth: 2,
        borderColor: colors.basePurple,
        backgroundColor: colors.lightPurple,
        width: '80%',
        height: '40%',
    },
    text: {
        color: colors.basePurple,
        fontSize: 18,
    },
    wordArea: {
        borderBottomWidth: 2,
        borderColor: colors.basePurple,
        backgroundColor: colors.lightPurple,
        textAlign: 'center',
        width: '70%',
        height: '8%',
        margin: 20
    },
    word: {
        color: colors.basePurple,
        fontSize: 24,
        textAlign: 'center',
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
        fontSize: 18,
    },
});

export default TalkScreen