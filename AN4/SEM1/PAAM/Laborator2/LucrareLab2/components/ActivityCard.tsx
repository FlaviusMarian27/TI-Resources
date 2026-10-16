import { StyleSheet, Text, View } from 'react-native';

type ActivityCardProps = {
  title: string;
  description: string;
  category: string;
};

export default function ActivityCard({ title, description, category }: ActivityCardProps) {
  return (
    <View style={styles.card}>
      <Text style={styles.title}>{title}</Text>
      <Text style={styles.description}>{description}</Text>
      <Text style={styles.category}>{category}</Text>
    </View>
  );
}

const styles = StyleSheet.create({
  card: {
    backgroundColor: '#f2f4f8',
    borderRadius: 12,
    padding: 16,
    marginBottom: 12,
  },
  title: {
    fontSize: 18,
    fontWeight: 'bold',
  },
  description: {
    fontSize: 14,
    color: '#555',
    marginTop: 4,
  },
  category: {
    fontSize: 12,
    fontWeight: '600',
    color: '#2563eb',
    marginTop: 8,
  },
});