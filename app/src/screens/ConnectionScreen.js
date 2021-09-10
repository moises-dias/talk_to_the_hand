import React, { useState, useEffect } from 'react';
import { View, StyleSheet, Button, Image, Text, TouchableOpacity, FlatList } from 'react-native';

import BluetoothSerial from "react-native-bluetooth-serial";

const ConnectionScreen = props => {

    const btManager = BluetoothSerial//props.route.params.manager;

    const [btStatus, setBtStatus] = useState(false);
    const [isConnected, setIsConnected] = useState(false);
    const [isPaired, setIsPaired] = useState(false);
    const [pairedDevices, setPairedDevices] = useState([]);
    const [unpairedDevices, setUnpairedDevices] = useState([]);
    const [messages, setMessages] = useState('start');

    // YellowBox.ignoreWarnings([
    //     'Non-serializable values were found in the navigation state',
    // ]);

    useEffect(() => {
        console.log(btManager)
        getStatusFromDevice();
        // if (btStatus)
        getPairedList();

    }, [])

    const getStatusFromDevice = async () => {
        console.log('get status')
        const status = await btManager.isEnabled();
        setBtStatus(status);
    };
    
    const getPairedList = async () => {
        console.log('paired list')
        const paired = await btManager.list();
        setPairedDevices(paired)
        console.log('loaded paired')
        const unpaired = await btManager.discoverUnpairedDevices();
        setUnpairedDevices(unpaired)
        console.log(unpaired)
        console.log('loaded unpaired')
        //setPaired(devices[0]['name'])
    }

    const gloveIsPaired = () => {
        let glove = pairedDevices.filter(device => {
            return device.name == 'HC-06'
        })
        return glove.length > 0;
    }

    const removeDuplicates = list => {
        console.log('remove duplicates')
        let result = unpairedDevices.filter((item, index) => {
            return unpairedDevices.indexOf(item) == index;
        })
        console.log(result)
        return result
    }

    const renderGridItem = (item) => {
        return(
                <View>
                    <Text style={styles.textDevice}>{item.item.name}</Text>
                </View>
            );
        };

    const connectToGlove = async () => {
        console.log('get status')
        const hc = await btManager.connect('98:D3:71:FD:5A:D7');
        console.log(hc)
        console.log('conectado?')
        // setConnected(true);
    };

    const renderConnection = () => {
        console.log('render conectionnnnnnnn')
        if (isConnected) 
        {
            return (
                <View>
                    <Text style={styles.text}>
                        Seu dispositivo já está conectado à luva!
                    </Text>
                    
                </View>
            )
        }
        else 
        {
            return (
                <View>
                {console.log('nao ta conectado')}
                    <Text style={styles.text}>
                        Clique no botão abaixo para se conectar:
                    </Text>
                    <View style={styles.button}>
                        <TouchableOpacity activeOpacity={0.4} onPress={() => {
                                console.log('conectar')
                                connectToGlove()
                            }}>
                            <Text style={styles.textButton}>conectar</Text>
                        </TouchableOpacity>
                    </View>
                </View>
            )
        }
    }

    const renderDevices = () => {
        console.log('render list')
        
        if (gloveIsPaired())
        {
            return (
                <View style={styles.deviceList}>
                    <Text style={styles.text}>
                        Seu dispositivo já está pareado com a luva!
                    </Text>                                        
                    {/* {renderConnection()} era pra essa merda funcionar >:(*/}
                    
                </View>
            )
        }
        else 
        {
            return (
            <View style={styles.deviceList}>
                    <Text style={styles.text}>
                        Procure o dispositivo HC-06 na lista abaixo e selecione-o para realizar o pareamento:
                    </Text> 
                <FlatList
                    keyExtractor={(item, index) => item.address} 
                    data={removeDuplicates(unpairedDevices)} 
                    renderItem={renderGridItem} 
                />
                </View>
            )
        }
    }
    
    const renderConnectionProcess = () => {
        if (btStatus) {
            return (
                <View style={styles.btStatus}>
                    <Text style={styles.text}>
                        Seu bluetooth está ativado!
                    </Text>
                {renderDevices()}
                {gloveIsPaired() ? renderConnection() : console.log('not paired')}
                </View>
            )
        }
        else {
            return (
                <View style={styles.btStatus}>
                    <Text style={styles.text}>
                        O bluetooth do seu dispositivo está desativado :( {'\n\n'} Ative o bluetooth e recarregue essa tela para se conectar à luva
                    </Text>
                </View>
            )
        }
    }

    return (
        <View style={styles.screen}>

            <View style={styles.container}>

                {renderConnectionProcess()}

            </View>
                    
            <View style={styles.buttonContainer}>
                <View style={styles.button}>
                    <TouchableOpacity activeOpacity={0.4} onPress={() => {
                            props.navigation.goBack();
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
        borderColor: "black",
    },
    container: {
        flex: 1,
        justifyContent: 'space-around',
        alignItems: 'center',
        width: '100%',
        borderWidth: 2,
        borderColor: 'red'
    },
    text: {
        color: '#300055',
        fontWeight: 'bold',
        fontSize: 20,
        textAlign: 'center',
        borderWidth: 10,
        borderColor: 'white',
        marginTop: 30
    },
    btStatus: {
        // marginTop: 50,
        paddingHorizontal: 50,
        // flex: 1,
        // justifyContent: 'center',
        borderWidth: 2,
        borderColor: 'green'
    },
    deviceList: {
        // width: '100%',
        maxHeight: '70%',
        borderWidth: 2,
        borderColor: 'blue',
    },
    textDevice: {
        color: '#300055',
        fontWeight: 'bold',
        fontSize: 20,
        textAlign: 'center',
        backgroundColor: 'rgba(245,233,254,0.6)',
        // height: '10%',
        padding: 20,
        margin: 10,
        // borderWidth: 2,
        borderColor: 'blue',
        // padding: 10,
    },
    conect: {

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
        backgroundColor: "#300055",
        borderWidth: 1,
        borderRadius: 6,
        borderColor: "white",
    },
});

export default ConnectionScreen