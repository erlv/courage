import tensorflow as tf
x = tf.constant([35,40,45], name='x')
print(x)
y=tf.Variable(x+5, name='y')
#model = tf.initialize_all_variables()
with tf.Session() as session:
	merged = tf.merge_all_summaries()
	writer = tf.train.SummaryWriter("/tmp/basic", session.graph)
	model = tf.initialize_all_variables()
	session.run(model)
	print(session.run(y))
