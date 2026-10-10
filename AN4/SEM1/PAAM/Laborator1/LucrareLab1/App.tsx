import { Button, StyleSheet, Text, View } from 'react-native';
import { useState } from 'react';

export default function App() {
  const [mesaj, setMesaj] = useState('Laborator PAAM')

  return (
    <View style={styles.container}>
      <Text style={styles.titlu}> {mesaj} </Text>
      <Text style={styles.nume}> Marian Flavius-Andrei </Text>
    
      <View style={styles.spatiu}> </View>

      <Button 
        title="Apasa aici"
        onPress={() => setMesaj('Salut din React Native!')}>
      </Button>

    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    backgroundColor: '#fff',
    alignItems: 'center',
    justifyContent: 'center',
  },

  titlu: {
    fontSize: 24,
    fontWeight: 'bold'
  },

  nume: {
    fontSize: 18,
    marginTop: 8
  },

  spatiu: {
    height: 24,
  }
});
