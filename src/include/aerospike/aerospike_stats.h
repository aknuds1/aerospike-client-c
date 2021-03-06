/*
 * Copyright 2008-2018 Aerospike, Inc.
 *
 * Portions may be licensed to Aerospike, Inc. under one or more contributor
 * license agreements.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"); you may not
 * use this file except in compliance with the License. You may obtain a copy of
 * the License at http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
 * License for the specific language governing permissions and limitations under
 * the License.
 */
#pragma once

#include <aerospike/aerospike.h>
#include <aerospike/as_node.h>

/**
 * @defgroup cluster_stats Cluster Statistics
 *
 * Statistics for each Aerospike client instance.
 */

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************
 * TYPES
 *****************************************************************************/

/**
 * Connection statistics.
 * @ingroup cluster_stats
 */
typedef struct as_conn_stats_s {
	/**
	 * Connections residing in pool(s) on this node.
	 * There can be multiple pools per node. This value is a summary of those pools on this node.
	 */
	uint32_t in_pool;

	/**
	 * Connections actively being used in database transactions on this node.
	 * There can be multiple pools per node. This value is a summary of those pools on this node.
	 */
	uint32_t in_use;

} as_conn_stats;

/**
 * Node statistics.
 * @ingroup cluster_stats
 */
typedef struct as_node_stats_s {
	/**
	 * Node.
	 */
	as_node* node;

	/**
	 * Sync connection statistics on this node.
	 */
	as_conn_stats sync;

	/**
	 * Async connection statistics on this node.
	 */
	as_conn_stats async;

	/**
	 * Async pipeline connection statistics on this node.
	 */
	as_conn_stats pipeline;

} as_node_stats;

/**
 * Cluster statistics.
 * @ingroup cluster_stats
 */
typedef struct as_cluster_stats_s {
	/**
	 * Statistics for all nodes.
	 */
	as_node_stats* nodes;

	/**
	 * Node count.
	 */
	uint32_t nodes_size;

	/**
	 * Count of sync batch/scan/query tasks awaiting execution. If the count is greater than zero,
	 * then all threads in the thread pool are active.
	 */
	uint32_t thread_pool_queued_tasks;

} as_cluster_stats;

struct as_cluster_s;

/******************************************************************************
 * FUNCTIONS
 *****************************************************************************/

/**
 * Retrieve aerospike cluster statistics.
 *
 * @param cluster	The aerospike cluster.
 * @param stats		The statistics summary for specified cluster.
 *
 * @ingroup cluster_stats
 */
AS_EXTERN void
aerospike_cluster_stats(struct as_cluster_s* cluster, as_cluster_stats* stats);
	
/**
 * Retrieve aerospike client instance statistics.
 *
 * ~~~~~~~~~~{.c}
 * as_cluster_stats stats;
 * aerospike_stats(&as, &stats);
 * aerospike_stats_destroy(&stats);
 * ~~~~~~~~~~
 *
 * @param as		The aerospike instance.
 * @param stats		The statistics summary for specified client instance.
 *
 * @ingroup cluster_stats
 */
static inline void
aerospike_stats(aerospike* as, as_cluster_stats* stats)
{
	aerospike_cluster_stats(as->cluster, stats);
}

/**
 * Release node references and memory allocated in aerospike_stats().
 *
 * @param stats		The cluster statistics summary.
 *
 * @ingroup cluster_stats
 */
AS_EXTERN void
aerospike_stats_destroy(as_cluster_stats* stats);

/**
 * Retrieve aerospike node statistics.
 *
 * @param node		The server node.
 * @param stats		The statistics summary for specified node.
 *
 * @ingroup cluster_stats
 */
AS_EXTERN void
aerospike_node_stats(as_node* node, as_node_stats* stats);

/**
 * Release node reference allocated in aerospike_node_stats().
 *
 * @param stats		The statistics summary for specified node.
 *
 * @ingroup cluster_stats
 */
static inline void
aerospike_node_stats_destroy(as_node_stats* stats)
{
	as_node_release(stats->node);
}

#ifdef __cplusplus
} // end extern "C"
#endif
